#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>  
#include <type_traits>
#include <new>
#include <stdio.h>

#include "utility.hpp"
#include "reflection/reflection.hpp"

static void Benchmark(refl::reflector&);
static void __UnloadLib(refl::reflector& r);
static void __LoadLib(refl::reflector& r);
static void __GenerateLib(const std::string&, const std::string&, refl::reflector& r);
static dllptr lib = 0;

int main() {
	std::vector<std::string> scriptFiles = {"TestScript.h", "ExampleScript.h"};
	refl::reflector reflect = refl::reflector();
	reflect.SetErrorCallback([](const char* c) { std::cout << c << std::endl; });
	reflect.SetOutputDir((files::GetParentExecuteableDir(3) + std::string("tests/scripts/Generated/")).c_str());

	while (true) {
		std::cout << "Enter Command: ";
		char userInput;
		std::cin >> userInput;
		if (userInput == 'e') {
			if (lib) {
				Benchmark(reflect);
				continue;
			}
			std::cout << "LIB NOT LOADED" << std::endl;
		}
		if (userInput == 'r') {
			if (lib) {
				__UnloadLib(reflect);
			}
			reflect.Clear();
			for (const std::string& s : scriptFiles)
				__GenerateLib( (files::GetParentExecuteableDir(3) + "tests/scripts/"), s, reflect);
			std::string buildOut = sys::build_proj(files::GetParentExecuteableDir(3), "GenerateMake");
			if (buildOut.size() > 0) {
				std::cout << buildOut << std::endl;
			}
			std::cout << "Build Complete...\n" << std::endl;
			std::string compileOut = sys::compile_proj(files::GetParentExecuteableDir(3), "Reflection_Tests_Scripts");
			if (compileOut.size() > 0) {
				std::cout << compileOut << std::endl;
			} 
			std::cout << "Compile Complete...\n" << std::endl;
			__LoadLib(reflect);
		}
		if (userInput == 'l') {
			__LoadLib(reflect);
		}
		if (userInput == 'u') {
			__UnloadLib(reflect);
		}
		if (userInput == 'g') {
			reflect.Clear();
			for (const std::string& s : scriptFiles)
				__GenerateLib( (files::GetParentExecuteableDir(3) + "tests/scripts/"), s, reflect);
		}
		if (userInput == 'q') {
			if (lib) __UnloadLib(reflect);
			break;
		}
	}
	return 0;
};

static void Benchmark(refl::reflector& reflect) {

	refl::store::storage* storage = reflect.GetStorage();
	//reflect.LoadGeneratedFiles();

	const std::unordered_map<std::string, refl::store::uobject_struct>& map = storage->get_map();
	std::cout << "PRINTING MAP : " << map.size() << std::endl;

	for (const std::pair<std::string, refl::store::uobject_struct>& p : map) {
		std::string s = p.first;
		for (const std::pair<std::string, refl::store::uproperty_struct>& _p : p.second.property_map) {
			s += " " + _p.second.name + ", " + _p.second.type_name + ", " + std::to_string(_p.second.offset) + " | ";
		}
		s += "\n";
		for (const std::pair<std::string, refl::store::ufunction_struct>& _p : p.second.function_map) {
			s += _p.first + "\n";
		}
		std::cout << s << std::endl;
	}
	std::cout << "MAP PRINTED" << std::endl;
	double ns = 0;
	double n = 0;
	{
		uint64_t time = GetTimeNS();
		refl::uClass uClss = reflect.CreateUClass("TestScript");
		std::cout << "HERE" << std::endl;
		//void* v1 = uClss.data();

		int getNum = uClss.CallFunction<int>("GetNumber", 823, false); 
		int* getInt2 = uClss.CallFunction<int*>("GetInt2");
		std::vector<int> vec = uClss.GetMember<std::vector<int>>("vec");
		for (int i = 0; i < vec.size(); i++) {
			std::cout << "VEC["<<i<<"]= " << vec[i] << std::endl;
		}
		
		*getInt2 = 55;

		std::string getString = uClss.CallFunction<std::string>("GetString");
		std::string* getStringPtr = uClss.CallFunction<std::string*>("GetStringPtr");
		*getStringPtr = "NEW STRING";
		getStringPtr = uClss.CallFunction<std::string*>("GetStringPtr");

		refl::uClass exampleScript = uClss.CallFunction<refl::uClass>("GetExampleScript");
		std::cout << "UCLASS RETREIEVED" << std::endl;
		std::string n = exampleScript.GetMember<std::string>("name");
		uint32_t id = exampleScript.GetMember<uint32_t>("id");
		std::cout <<"Example Script: " << id << " name: " << n << std::endl;

		time = GetTimeNS() - time;
		//std::cout << "\n\n" << (double)time / 1e6 << " ms | " << time << " ns\n\n" << std::endl;
		ns += (double)time / 1e6;
	}
	std::cout << "REFLECTION: " << ns  << "ms" << std::endl;

	//std::cout << (ns < n ? "REFLECTION " : "NATIVE ") << " is " << (ns < n ? n / ns : ns / n) << "x faster" << std::endl;

	//reflect.UnloadGeneratedFiles();
}


static void __LoadLib(refl::reflector& r) {
	std::string loc = files::GetParentExecuteableDir(0) + dll::GetDLLExtensionName("cpy_Reflection_Tests_Scripts");
	std::ifstream src(files::GetParentExecuteableDir(0) + dll::GetDLLExtensionName("Reflection_Tests_Scripts"), std::ios::binary);
#ifdef _WIN32
	dll::movePDB(files::GetParentExecuteableDir(0), "Reflection_Tests_Scripts");
#endif
	std::ofstream  dst(loc, std::ios::binary);
	dst << src.rdbuf();
	dst.close();
	lib = dll::dlopen(loc.c_str(), 0);
	if (!lib) {
		std::cout << "INVALID HANDLE: " << dll::dlerror() << std::endl;
		return;
	}
	
	void (*func_ptr)(::refl::store::storage*) = reinterpret_cast<void (*)(::refl::store::storage*)>(dll::dlsym(lib, "__ReflectionMap__loadGeneratedFiles"));
	if (!func_ptr) {
		std::cout << "COULD NOT LOAD SYMBOL" << std::endl;
		dll::dlclose(lib);
		lib = 0;
		return;
	}
	(*func_ptr)(r.GetStorage());
}

static void __UnloadLib(refl::reflector& r) {
	if (lib) {
		void (*func_ptr)(::refl::store::storage*) = reinterpret_cast<void (*)(::refl::store::storage*)>(dll::dlsym(lib, "__ReflectionMap__unloadGeneratedFiles"));
		if (func_ptr) {
			(*func_ptr)(r.GetStorage());
		}
		dll::dlclose(lib);
		lib = 0;
		std::string loc = files::GetParentExecuteableDir(0) + dll::GetDLLExtensionName("cpy_Reflection_Tests_Scripts");
		::remove(loc.c_str());
	}
}

static void __GenerateLib(const std::string& path, const std::string& name, refl::reflector& r) {
	std::string in = path+name;
	std::ifstream t(in);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::cout << in << std::endl;
	r.Generate(in.c_str());
	if (r.HasError()) {
		std::cout << r.GetError() << std::endl;
	}
}

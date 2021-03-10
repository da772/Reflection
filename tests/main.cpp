#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>  
#include <type_traits>
#include <new>

#include "utility.hpp"
#include "reflection/reflection.hpp"

#include "scripts/TestScript.h"

#define ADDR(x) reinterpret_cast<uintptr_t>(x)




template<typename T>
T GetPtrVar(::refl::store::storage* store, void* v, const std::string& clazz, const std::string& name) {
	const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
	uintptr_t o = map.at(clazz).property_map.at(name).offset;
	return *(T*)((uint8_t*)v + o);
}

template<typename T>
void SetPtrVar(::refl::store::storage* store,void* v, const std::string& clazz, const std::string& name, T value) {
	const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
	uintptr_t o = map.at(clazz).property_map.at(name).offset;
	*(T*)((uint8_t*)v + o) = value;
}


template <typename T, typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr>
T _CallPtrFunction(::refl::store::storage* store, void* v, const std::string& clazz, const std::string& name, const std::vector<void*>& vec) {
	const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
	
	std::cout<< name << " NOT POINTER" << std::endl;
	T* _f = (T*)map.at(clazz).function_map.at(name).function(v, vec);
	T f = *_f;
	delete _f;
	return f;
}

template <typename T, typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr>
T _CallPtrFunction(::refl::store::storage* store, void* v, const std::string& clazz, const std::string& name, const std::vector<void*>& vec) {
	const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();

	std::cout<< name << " POINTER" << std::endl;
	void* _f = map.at(clazz).function_map.at(name).function(v, vec);
	T f = (T)_f;
	return f;

}

template<typename T, typename ... Args>
T CallPtrFunction(::refl::store::storage* store,void* v, const std::string& clazz, const std::string& name, Args&& ... args) {
	std::vector<void*> vec = {(void*)&args...};
	return _CallPtrFunction<T>(store, v, clazz, name, vec);
}

template<typename ... Args >
void CallPtrFunction(::refl::store::storage* store, void* v, const std::string& clazz, const std::string& name, Args&& ... args) {
	const std::unordered_map<std::string, refl::store::uobject_struct>& map = store->get_map();
	std::vector<void*> vec = {(void*)&args...};
	void* f = map.at(clazz).function_map.at(name).function(v, vec);
	::refl::store::uproperty_type ret_type = map.at(clazz).function_map.at(name).ret_val;
	if (ret_type != ::refl::store::uproperty_type::_ptr && ret_type != ::refl::store::uproperty_type::uclass_ptr && ret_type != ::refl::store::uproperty_type::_void) {
		if (f != nullptr)
			free(f);
	}
	return;
}

static void Benchmark();

int main() {
	
	Benchmark();

	return 0;
};

static void Benchmark() {
	refl::reflector reflector = refl::reflector();

	reflector.SetErrorCallback([](const char* c) { std::cout << c << std::endl; });
	reflector.SetOutputDir((GetParentExecuteableDir(0) + std::string("tests/scripts/Generated/")).c_str());

	std::string in = GetParentExecuteableDir(0) + "tests/scripts/TestScript.h";
	std::ifstream t(in);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::cout << in << std::endl;
#if 0
	//std::string out = GetParentExecuteableDir(0)+"scripts/TestScript.generated.h";
	std::cout << in << std::endl;
	reflector.Generate(in.c_str());
	if (reflector.HasError()) {
		std::cout << reflector.GetError();
	}
	//std::cout << (out ? out : reflector->GetError()) << std::endl;
	// RELOAD DLL

#else
	refl::store::storage* storage = reflector.GetStorage();
	reflector.LoadGeneratedFiles();

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
	double ns = 0;
	double n = 0;
	int testSize = 100000;
	for (int i = 0; i < testSize; i++) {
		{
			uint64_t time = GetTimeNS();
			refl::uClass uClss = reflector.CreateUClass("TestScript");
			//void* v1 = uClss.data();

			int getNum = uClss.CallFunction<int>("GetNumber", 823, false); //CallPtrFunction<int>(storage, v1, "TestScript", "GetNumber", 823, false);
			int* getInt2 = uClss.CallFunction<int*>("GetInt2");//CallPtrFunction<int*>(storage, v1, "TestScript", "GetInt2");
			//std::cout << "GET NUM = " << std::to_string(getNum) << std::endl;

			//std::cout << "GET INT = " << std::to_string(*getInt2) << std::endl;
			*getInt2 = 55;
			//std::cout << "GET INT = " << std::to_string(*getInt2) << std::endl;


			std::string getString = uClss.CallFunction<std::string>("GetString");
			//std::cout << getString << std::endl;
			std::string* getStringPtr = uClss.CallFunction<std::string*>("GetStringPtr");
			//std::cout << *getStringPtr << std::endl;
			*getStringPtr = "NEW STRING";
			getStringPtr = uClss.CallFunction<std::string*>("GetStringPtr");
			//std::cout << *getStringPtr << std::endl;
			TestStruct getStruct = uClss.CallFunction<TestStruct>("GetStruct");
			//std::cout << ts->i << std::endl;
			time = GetTimeNS() - time;
			//std::cout << "\n\n" << (double)time / 1e6 << " ms | " << time << " ns\n\n" << std::endl;
			ns += (double)time / 1e6;
		}
		{
			uint64_t time = GetTimeNS();
			TestScript* t = new TestScript();
			int getNum = t->GetNumber(832, false);
			int* getInt2 = t->GetInt2();
			*getInt2 = 55;
			std::string getString = t->GetString();
			std::string* getStringPtr = t->GetStringPtr();
			*getStringPtr = "NEW STRING";
			getStringPtr = t->GetStringPtr();
			TestStruct getStruct = t->GetStruct();
			time = GetTimeNS() - time;
			n += (double)time / 1e6;
			//std::cout << "\n\n" << (double)time / 1e6 << " ms | " << time << " ns\n\n" << std::endl;
		}
	}
	std::cout << "REFLECTION: " << ns / (double)testSize << "ms NATIVE: " << n / (double)testSize << " ms" << std::endl;

	reflector.UnloadGeneratedFiles();
#endif
}

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>  
#include <type_traits>
#include <new>
#include <stdio.h>
#include <vector>

#include "utility.hpp"
#include "reflection/reflection.hpp"


static void ReloadLib(dllptr* lib, refl::reflector& reflect,  const std::vector<std::string>& scriptFiles);

int main() {
	dllptr lib = 0;
	std::vector<std::string> scriptFiles = {"TestScript.h", "ExampleScript.h", "MainScript.h"};
	refl::reflector reflect = refl::reflector();
	reflect.SetErrorCallback([](const char* c) { std::cout << c << std::endl; });
	reflect.SetOutputDir((files::GetParentExecuteableDir(3) + std::string("tests/scripts/Generated/")).c_str());
	ReloadLib(&lib, reflect, scriptFiles);
	while (true) {
		refl::uClass mainScript = reflect.CreateUClass("MainScript");
		mainScript.SetMember<refl::reflector*>("reflect", &reflect);
		int& code = mainScript.GetMember<int>("exitCode");
		mainScript.CallFunction<void>("Run");
		if (code == 0) {
			break;
		} else if (code == 1) {
			code = 0;
			ReloadLib(&lib, reflect, scriptFiles);
		}
	}

	return 0;
};

static void ReloadLib(dllptr* lib, refl::reflector& reflect, const std::vector<std::string>& scriptFiles) {
	std::cout << "PTR " << reinterpret_cast<uintptr_t>(*lib) << std::endl;
		if (*lib) {
			__UnloadLib(lib, reflect);
		}
		reflect.Clear();
		//for (const std::string& s : scriptFiles)
			//__GenerateLib((files::GetParentExecuteableDir(3) + "tests/scripts/"), s, reflect);
		std::string buildOut = sys::build_proj(files::GetParentExecuteableDir(3), "GenerateMake");
		if (buildOut.size() > 0) {
			std::cout << buildOut << std::endl;
		}
		std::cout << "Build Complete...\n" << std::endl;
		std::string compileOut = sys::compile_proj(files::GetParentExecuteableDir(3), "Reflection_Tests_Scripts");
		if (compileOut.size() > 0) {
			//std::cout << compileOut << std::endl;
		} 
		std::cout << "Compile Complete...\n" << std::endl;
		__LoadLib(lib,reflect);
		std::cout << "LOAD DLL" << std::endl;
}




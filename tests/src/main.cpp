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


static bool ReloadLib(dllptr* lib, refl::reflector& reflect,  const std::vector<std::string>& scriptFiles);

int main() {
	dllptr lib = 0;
	std::vector<std::string> scriptFiles = {"TestScript.h", "ExampleScript.h", "MainScript.h"};
	refl::reflector reflect = refl::reflector();
	reflect.SetErrorCallback([](const char* c) { std::cout << c << std::endl; });
	reflect.SetOutputDir((files::GetParentExecuteableDir(3) + std::string("tests/scripts/Generated/")).c_str());
	bool init = ReloadLib(&lib, reflect, scriptFiles);
	while (!init) {
		char s = 0;
		std::cout << "COMPILE FAILED PRESS ENTER TO TRY AGAIN OR q TO QUIT" << std::endl;
		s = std::cin.get();
		if (s == 'q')
			return 0;
		init = ReloadLib(&lib, reflect, scriptFiles);
	}
	refl::uClass mainScript = reflect.CreateUClass("MainScript");
	mainScript.SetMember<refl::reflector*>("reflect", &reflect);
	int& code = mainScript.GetMember<int>("exitCode");
	mainScript.SetMember<std::vector<std::string>*>("scriptFiles", &scriptFiles);
	while (true) {	
		mainScript.CallFunction<void>("Run");
		if (code == 0) {
			break;
		} else if (code == 1) {
			code = 0;
			bool reload = ReloadLib(&lib, reflect, scriptFiles);
			while (!reload) {
				char s = 0;
				std::cout << "COMPILE FAILED PRESS ENTER TO TRY AGAIN OR q TO QUIT" << std::endl;
				s = std::cin.get();
				if (s == 'q')
					return 0;
				reload = ReloadLib(&lib, reflect, scriptFiles);
			}
		}
	}

	return 0;
};

static bool ReloadLib(dllptr* lib, refl::reflector& reflect, const std::vector<std::string>& scriptFiles) {
		reflect.Clear();
		for (const std::string& s : scriptFiles)
			__GenerateLib((files::GetParentExecuteableDir(3) + "tests/scripts/"), s, reflect);
		std::string buildOut = sys::build_proj(files::GetParentExecuteableDir(3), "GenerateMake");
		if (buildOut.size() > 0) {
			std::cout << buildOut << std::endl;
		}
		std::cout << "Build Complete...\n" << std::endl;
		std::string compileOut = sys::compile_proj(files::GetParentExecuteableDir(3), "Reflection_Tests_Scripts");
		if (compileOut.size() > 0) {
			if (compileOut.find("error") != std::string::npos) {
				std::cout << compileOut << std::endl;
				return false;
			}
		}
		if (*lib) {
			__UnloadLib(lib, reflect);
		}
		std::cout << "Compile Complete...\n" << std::endl;
		__LoadLib(lib,reflect);
		std::cout << "LOAD DLL" << std::endl;
		return true;
}




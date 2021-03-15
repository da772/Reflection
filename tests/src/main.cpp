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

#ifdef TESTS_LINK_TYPE
	#if (TESTS_LINK_TYPE == 0)
		#include "Reflection.map.generated.h"
	#endif
#endif


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
	
	while (true) {	
		int* code;
		{
			refl::uClass mainScript = reflect.CreateUClass("MainScript");
			mainScript.SetMember<refl::reflector*>("reflect", &reflect);
			code = &mainScript.GetMember<int>("exitCode");
			mainScript.SetMember<std::vector<std::string>*>("scriptFiles", &scriptFiles);
			mainScript.CallFunction<void>("Run");
		}
		if (*code == 0) {
			break;
		} else if (*code == 1) {
			*code = 0;
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
#if (TESTS_LINK_TYPE == 1)
		if (*lib) {
			__UnloadLib(lib, reflect);
		}
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
				if (compileOut.find("insufficient") != std::string::npos) {
					std::cout << "IS DEBUGGER ATTATCHED?" << std::endl;
				}
				return false;
			}
		}
		std::cout << "Compile Complete...\n" << std::endl;
		__LoadLib(lib,reflect);
		std::cout << "LOAD DLL" << std::endl;
		return true;
#else
	if (*lib) {
		__ReflectionMap__unloadGeneratedFiles(reflect.GetStorage());
		*lib = (dllptr)0;
	}
	__ReflectionMap__loadGeneratedFiles(reflect.GetStorage());
	*lib = (dllptr)1;
	return true;
#endif
}




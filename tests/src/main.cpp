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

#ifndef HOT_RELOAD
	#include "Reflection.map.generated.h"
#endif


static bool ReloadLib(dllptr* lib, refl::reflector& reflect,  const std::vector<std::string>& scriptFiles);

int main() {

	dllptr lib = 0;
	std::vector<std::string> scriptFiles = {"TestScript.h", "ExampleScript.h", "MainScript.h", "StaticScript.h"};
	refl::reflector reflect = refl::reflector();
	reflect.SetErrorCallback([](const char* c) { std::cout << c << std::endl; });

	reflect.SetOutputDir((files::GetParentExecuteableDir(2) + std::string("tests/scripts/Generated/")).c_str());
	reflect.SetRelativeInclude("../");
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
		int code;
		{
			refl::uClass mainScript = reflect.CreateUClass("MainScript");
			mainScript.SetMember<refl::reflector*>("reflect", &reflect);
			mainScript.SetMember<std::vector<std::string>*>("scriptFiles", &scriptFiles);
			mainScript.CallFunction<void>("Run");
			code = mainScript.GetMember<int>("exitCode");
		}
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
#ifdef HOT_RELOAD
		if (*lib) {
			__UnloadLib(lib, reflect);
		}
		reflect.Clear();
		for (const std::string& s : scriptFiles) {
			__GenerateLib((files::GetParentExecuteableDir(2) + "tests/scripts/"), s, reflect);
		}
		//std::string buildOut = sys::build_proj(files::GetParentExecuteableDir(3), "GenerateMake");
		//if (buildOut.size() > 0) {
		//	std::cout << buildOut << std::endl;
		//}
		std::cout << "Build Complete...\n" << std::endl;
		std::string pathDir = files::GetParentExecuteableDir(1);
		#if defined(_WIN32) && !defined(COMPILER)
		pathDir = files::GetParentExecuteableDir(2) + "scripts/";
		#endif
		std::string compileOut = sys::compile_proj(pathDir, "Scripts", "make");
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
		__LoadLib(lib,reflect, "Scripts");
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




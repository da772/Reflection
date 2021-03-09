#include "utility.hpp"
#include "reflection/reflection.hpp"


#include "scripts/Generated/TestScript.generated.h"

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>  

int main() {
	refl::reflector* reflector = refl::reflector::Get();
	std::string in = GetParentExecuteableDir(0)+"tests/scripts/TestScript.h";
	std::ifstream t(in);
	std::stringstream buffer;
	buffer << t.rdbuf();
	//std::string out = GetParentExecuteableDir(0)+"scripts/TestScript.generated.h";
	std::cout << in << std::endl;
	const char* out = reflector->GenerateM(buffer.str().c_str());
	std::cout << (out ? out : reflector->GetError()) << std::endl;

	TestScript_Generated g;
	g.Load();
	
	g.Unload();

	if (out)
		delete out;
	return 0;
}

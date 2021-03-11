#include "Reflection.map.generated.h"
#include "TestScript.generated.h"
#include <iostream>

void ReflectionMap__loadGeneratedFiles(refl::store::storage* storage) {
	TestScript_Generated::Load(storage);
	std::cout << "MAP LOADED" << std::endl;
}

void ReflectionMap__unloadGeneratedFiles(refl::store::storage* storage) {
	TestScript_Generated::Unload(storage);
	std::cout << "MAP UNLOADED" << std::endl;
}

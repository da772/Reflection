#include "Reflection.map.generated.h"
#include "TestScript.generated.h"
#include "ExampleScript.generated.h"


	void __ReflectionMap__loadGeneratedFiles(::refl::store::storage* storage) {
		TestScript_Generated::Load(storage);
		ExampleScript_Generated::Load(storage);
	}
	void __ReflectionMap__unloadGeneratedFiles(::refl::store::storage* storage) {
		TestScript_Generated::Unload(storage);
		ExampleScript_Generated::Unload(storage);
	}


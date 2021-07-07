#include "Reflection.map.generated.h"
#include "ClassTwo.generated.h"
#include "AnothaClass.generated.h"
#include "TestScript.generated.h"
#include "MostBasedClass.generated.h"
#include "MainScript.generated.h"
#include "MoreBasedClass.generated.h"
#include "BaseClass.generated.h"
#include "ExampleScript.generated.h"
#include "StaticClass.generated.h"


	void __ReflectionMap__loadGeneratedFiles(::refl::store::storage* storage) {
		ClassTwo_Generated::Load(storage);
		AnothaClass_Generated::Load(storage);
		TestScript_Generated::Load(storage);
		MostBasedClass_Generated::Load(storage);
		MainScript_Generated::Load(storage);
		MoreBasedClass_Generated::Load(storage);
		BaseClass_Generated::Load(storage);
		ExampleScript_Generated::Load(storage);
		StaticClass_Generated::Load(storage);
	}
	void __ReflectionMap__unloadGeneratedFiles(::refl::store::storage* storage) {
		ClassTwo_Generated::Unload(storage);
		AnothaClass_Generated::Unload(storage);
		TestScript_Generated::Unload(storage);
		MostBasedClass_Generated::Unload(storage);
		MainScript_Generated::Unload(storage);
		MoreBasedClass_Generated::Unload(storage);
		BaseClass_Generated::Unload(storage);
		ExampleScript_Generated::Unload(storage);
		StaticClass_Generated::Unload(storage);
	}


#include "Reflection.map.generated.h"
#include "TestScript.generated.h"

namespace refl {
	void LoadGeneratedFiles() {
		TestScript_Generated::Load();
	}
	void UnloadGeneratedFiles() {
		TestScript_Generated::Unload();
	}
}

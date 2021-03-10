#include "Reflection.map.generated.h"
#include "TestScript.generated.h"

namespace refl {
	void ::refl::impl::__loadGeneratedFiles(::refl::store::storage* storage) {
		TestScript_Generated::Load(storage);
	}
	void ::refl::impl::__unloadGeneratedFiles(::refl::store::storage* storage) {
		TestScript_Generated::Unload(storage);
	}
}

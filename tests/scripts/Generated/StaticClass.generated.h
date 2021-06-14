#pragma once
#include "reflection/reflection.hpp"
#include "../StaticScript.h"
class StaticClass_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("StaticClass",{"StaticClass",{
		{"r",{"r","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&StaticClass::r) }}}, {
		{"StaticFunction",{"StaticFunction","int",static_cast<refl::store::uproperty_type>(5),{},[](void* ptr, std::vector<void*> args) {int v = StaticClass::StaticFunction(); int* _ptr = new int(v); return (void*)_ptr; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("StaticClass");
	}
};

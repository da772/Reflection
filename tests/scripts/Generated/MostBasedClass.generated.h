#pragma once
#include "reflection/reflection.hpp"
#include "../ExampleScript.h"
class MostBasedClass_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("MostBasedClass",{"MostBasedClass",{
		{"mostBasedProperty",{"mostBasedProperty","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&MostBasedClass::mostBasedProperty) }}}, {
		{"MostBasedFunc",{"MostBasedFunc","int",static_cast<refl::store::uproperty_type>(5),{},[](void* ptr, std::vector<void*> args) {int v = (*((MostBasedClass*)ptr).*(&MostBasedClass::MostBasedFunc))(); int* _ptr = new int(v); return (void*)_ptr; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("MostBasedClass");
	}
};

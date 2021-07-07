#pragma once
#include "reflection/reflection.hpp"
#include "../ExampleScript.h"
class MoreBasedClass_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("MoreBasedClass",{"MoreBasedClass",{
		{"basedProperty",{"basedProperty","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&MoreBasedClass::basedProperty) }},
		{"mostBasedProperty",{"mostBasedProperty","int",static_cast<refl::store::uproperty_type>(5),offsetof(MoreBasedClass,mostBasedProperty) }},
}, {
		{"BasedFunc",{"BasedFunc","int",static_cast<refl::store::uproperty_type>(5),{},[](void* ptr, std::vector<void*> args) {int v = (*((MoreBasedClass*)ptr).*(&MoreBasedClass::BasedFunc))(); int* _ptr = new int(v); return (void*)_ptr; } }},
		{"MostBasedFunc",{"MostBasedFunc","int",static_cast<refl::store::uproperty_type>(5),{},[](void* ptr, std::vector<void*> args) {int v = (*((MoreBasedClass*)ptr).*(&MoreBasedClass::MostBasedFunc))(); int* _ptr = new int(v); return (void*)_ptr; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("MoreBasedClass");
	}
};

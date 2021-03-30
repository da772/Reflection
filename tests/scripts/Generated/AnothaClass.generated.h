#pragma once
#include "reflection/reflection.hpp"
#include "../TestScript.h"
class AnothaClass_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("AnothaClass",{"AnothaClass",{
		{"myArr",{"myArr","std::vector<int>",static_cast<refl::store::uproperty_type>(1),refl::store::offsetOf(&AnothaClass::myArr) }}}, {
		{"AnothaClass",{"AnothaClass","AnothaClass",static_cast<refl::store::uproperty_type>(1),{},[](void* ptr, std::vector<void*> args) {return (void*)new AnothaClass();} }},
		{"~AnothaClass",{"~AnothaClass","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {AnothaClass* p = (AnothaClass*)ptr; delete p; return nullptr;} }},
		{"AnotheFunction",{"AnotheFunction","int",static_cast<refl::store::uproperty_type>(5),{},[](void* ptr, std::vector<void*> args) {int v = (*((AnothaClass*)ptr).*(&AnothaClass::AnotheFunction))(); int* _ptr = new int(v); return (void*)_ptr; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("AnothaClass");
	}
};

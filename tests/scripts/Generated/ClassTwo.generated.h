#pragma once
#include "reflection/reflection.hpp"
#include "../TestScript.h"
class ClassTwo_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("ClassTwo",{"ClassTwo",{
		{"i",{"i","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&ClassTwo::i) }},
		{"testProperty",{"testProperty","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&ClassTwo::testProperty) }}}, {
		{"ClassTwo",{"ClassTwo","ClassTwo",static_cast<refl::store::uproperty_type>(1),{{static_cast<::refl::store::uproperty_type>(5),"int"} },[](void* ptr, std::vector<void*> args) {return (void*)new ClassTwo(*(int*)args[0]);} }},
		{"~ClassTwo",{"~ClassTwo","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {ClassTwo* p = (ClassTwo*)ptr; delete p; return nullptr;} }},
		{"TestFunc",{"TestFunc","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((ClassTwo*)ptr).*(&ClassTwo::TestFunc))(); return nullptr; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("ClassTwo");
	}
};

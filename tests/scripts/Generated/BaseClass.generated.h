#pragma once
#include "reflection/reflection.hpp"
#include "../ExampleScript.h"
class BaseClass_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("BaseClass",{"BaseClass",{
		{"parentProperty",{"parentProperty","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&BaseClass::parentProperty) }},
		{"basedProperty",{"basedProperty","int",static_cast<refl::store::uproperty_type>(5),offsetof(BaseClass,basedProperty) }},
		{"mostBasedProperty",{"mostBasedProperty","int",static_cast<refl::store::uproperty_type>(5),offsetof(BaseClass,mostBasedProperty) }},
}, {
		{"BaseClass",{"BaseClass","BaseClass",static_cast<refl::store::uproperty_type>(1),{},[](void* ptr, std::vector<void*> args) {return (void*)new BaseClass();} }},
		{"~BaseClass",{"~BaseClass","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {BaseClass* p = (BaseClass*)ptr; delete p; return nullptr;} }},
		{"ParentFunc",{"ParentFunc","int",static_cast<refl::store::uproperty_type>(5),{},[](void* ptr, std::vector<void*> args) {int v = (*((BaseClass*)ptr).*(&BaseClass::ParentFunc))(); int* _ptr = new int(v); return (void*)_ptr; } }},
		{"BasedFunc",{"BasedFunc","int",static_cast<refl::store::uproperty_type>(5),{},[](void* ptr, std::vector<void*> args) {int v = (*((BaseClass*)ptr).*(&BaseClass::BasedFunc))(); int* _ptr = new int(v); return (void*)_ptr; } }},
		{"MostBasedFunc",{"MostBasedFunc","int",static_cast<refl::store::uproperty_type>(5),{},[](void* ptr, std::vector<void*> args) {int v = (*((BaseClass*)ptr).*(&BaseClass::MostBasedFunc))(); int* _ptr = new int(v); return (void*)_ptr; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("BaseClass");
	}
};

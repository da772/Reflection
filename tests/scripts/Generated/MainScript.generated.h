#pragma once
#include "reflection/reflection.hpp"
#include "../MainScript.h"
class MainScript_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("MainScript",{"MainScript",{
		{"reflect",{"reflect","refl::reflector*",static_cast<refl::store::uproperty_type>(2),refl::store::offsetOf(&MainScript::reflect) }},
		{"m",{"m","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&MainScript::m) }},
		{"lib",{"lib","dllptr",static_cast<refl::store::uproperty_type>(1),refl::store::offsetOf(&MainScript::lib) }},
		{"scriptFiles",{"scriptFiles","std::vector<std::string>*",static_cast<refl::store::uproperty_type>(2),refl::store::offsetOf(&MainScript::scriptFiles) }},
		{"exitCode",{"exitCode","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&MainScript::exitCode) }}}, {
		{"MainScript",{"MainScript","MainScript",static_cast<refl::store::uproperty_type>(1),{},[](void* ptr, std::vector<void*> args) {return (void*)new MainScript();} }},
		{"~MainScript",{"~MainScript","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {MainScript* p = (MainScript*)ptr; delete p; return nullptr;} }},
		{"Run",{"Run","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((MainScript*)ptr).*(&MainScript::Run))(); return nullptr; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("MainScript");
	}
};

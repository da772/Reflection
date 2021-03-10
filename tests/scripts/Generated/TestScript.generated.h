#pragma once
#include "reflection/reflection.hpp"
#include "../TestScript.h"
class TestScript_Generated : public refl::class_generation {
 	public:
	inline static void Load() {
		refl::store::storage* storage = refl::reflector::Get()->GetStorage();
		storage->store("TestScript",{"TestScript",{
		{"int1",{"int1","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&TestScript::int1) }},
		{"int2",{"int2","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&TestScript::int2) }},
		{"numberptr",{"numberptr","int*",static_cast<refl::store::uproperty_type>(10),refl::store::offsetOf(&TestScript::numberptr) }},
		{"char1",{"char1","char",static_cast<refl::store::uproperty_type>(7),refl::store::offsetOf(&TestScript::char1) }},
		{"uint1",{"uint1","unsigned int",static_cast<refl::store::uproperty_type>(6),refl::store::offsetOf(&TestScript::uint1) }},
		{"uchar1",{"uchar1","unsigned char",static_cast<refl::store::uproperty_type>(8),refl::store::offsetOf(&TestScript::uchar1) }},
		{"uint64",{"uint64","uint64_t",static_cast<refl::store::uproperty_type>(18),refl::store::offsetOf(&TestScript::uint64) }},
		{"vec",{"vec","std::vector<int>",static_cast<refl::store::uproperty_type>(1),refl::store::offsetOf(&TestScript::vec) }},
		{"bBool",{"bBool","bool",static_cast<refl::store::uproperty_type>(4),refl::store::offsetOf(&TestScript::bBool) }},
		{"myStruct",{"myStruct","TestStruct",static_cast<refl::store::uproperty_type>(1),refl::store::offsetOf(&TestScript::myStruct) }}}, {
		{"constructor",{"constructor","TestScript",static_cast<refl::store::uproperty_type>(19),{},[](void* ptr, std::vector<void*> args) {return (void*)new TestScript();} }},
		{"~constructor",{"~constructor","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {TestScript* p = (TestScript*)ptr; delete p; return nullptr;} }},
		{"Update",{"Update","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::Update))(); return nullptr; } }},
		{"End",{"End","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::End))(); return nullptr; } }},
		{"Begin",{"Begin","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::Begin))(); return nullptr; } }},
		{"GetEntity",{"GetEntity","Entity*",static_cast<refl::store::uproperty_type>(2),{},[](void* ptr, std::vector<void*> args) {Entity* v = (*((TestScript*)ptr).*(&TestScript::GetEntity))(); return (void*)v; } }},
		{"GetNumber",{"GetNumber","int",static_cast<refl::store::uproperty_type>(5),{{static_cast<::refl::store::uproperty_type>(5),"int"},{static_cast<::refl::store::uproperty_type>(4),"bool"} },[](void* ptr, std::vector<void*> args) {int v = (*((TestScript*)ptr).*(&TestScript::GetNumber))(*(int*)args[0], *(bool*)args[1]); void* _ptr = malloc(sizeof(int)); memcpy(_ptr, &v, sizeof(int)); return _ptr; } }}}});
	}
	inline static void Unload() {
		refl::store::storage* storage = refl::reflector::Get()->GetStorage();
		storage->discard("TestScript");
	}
};

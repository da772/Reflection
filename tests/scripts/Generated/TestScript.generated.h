#pragma once
#include "reflection/reflection.hpp"
#include "../TestScript.h"
class TestScript_Generated : public refl::class_generation {
 	public:
	inline static void Load(::refl::store::storage* storage) {
		storage->store("TestScript",{"TestScript",{
		{"int1",{"int1","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&TestScript::int1) }},
		{"int2",{"int2","int",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&TestScript::int2) }},
		{"numberptr",{"numberptr","int*",static_cast<refl::store::uproperty_type>(2),refl::store::offsetOf(&TestScript::numberptr) }},
		{"char1",{"char1","char",static_cast<refl::store::uproperty_type>(7),refl::store::offsetOf(&TestScript::char1) }},
		{"uint1",{"uint1","unsigned int",static_cast<refl::store::uproperty_type>(6),refl::store::offsetOf(&TestScript::uint1) }},
		{"uchar1",{"uchar1","unsigned char",static_cast<refl::store::uproperty_type>(8),refl::store::offsetOf(&TestScript::uchar1) }},
		{"uint64",{"uint64","uint64_t",static_cast<refl::store::uproperty_type>(17),refl::store::offsetOf(&TestScript::uint64) }},
		{"vec",{"vec","std::vector<int>",static_cast<refl::store::uproperty_type>(1),refl::store::offsetOf(&TestScript::vec) }},
		{"bBool",{"bBool","bool",static_cast<refl::store::uproperty_type>(4),refl::store::offsetOf(&TestScript::bBool) }},
		{"myStruct",{"myStruct","TestStruct",static_cast<refl::store::uproperty_type>(1),refl::store::offsetOf(&TestScript::myStruct) }},
		{"myString",{"myString","std::string",static_cast<refl::store::uproperty_type>(1),refl::store::offsetOf(&TestScript::myString) }}}, {
		{"TestScript",{"TestScript","TestScript",static_cast<refl::store::uproperty_type>(1),{},[](void* ptr, std::vector<void*> args) {return (void*)new TestScript();} }},
		{"~TestScript",{"~TestScript","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) {TestScript* p = (TestScript*)ptr; delete p; return nullptr;} }},
		{"Update",{"Update","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::Update))(); return nullptr; } }},
		{"End",{"End","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::End))(); return nullptr; } }},
		{"Begin",{"Begin","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::Begin))(); return nullptr; } }},
		{"GetEntity",{"GetEntity","Entity*",static_cast<refl::store::uproperty_type>(2),{},[](void* ptr, std::vector<void*> args) {Entity* v = (*((TestScript*)ptr).*(&TestScript::GetEntity))(); return (void*)v; } }},
		{"GetExampleScript",{"GetExampleScript","ExampleScript*",static_cast<refl::store::uproperty_type>(2),{},[](void* ptr, std::vector<void*> args) {ExampleScript* v = (*((TestScript*)ptr).*(&TestScript::GetExampleScript))(); return (void*)v; } }},
		{"GetVoidPtr",{"GetVoidPtr","void*",static_cast<refl::store::uproperty_type>(9),{},[](void* ptr, std::vector<void*> args) {void* v = (*((TestScript*)ptr).*(&TestScript::GetVoidPtr))(); void** _ptr = new void*(v); return (void*)_ptr; } }},
		{"GetIntRef",{"GetIntRef","int&",static_cast<refl::store::uproperty_type>(19),{{static_cast<::refl::store::uproperty_type>(19),"int&"} },[](void* ptr, std::vector<void*> args) {void* v = &(*((TestScript*)ptr).*(&TestScript::GetIntRef))((int&)(*(int*)args[0])); return (void*)v; } }},
		{"Benchmark",{"Benchmark","void",static_cast<refl::store::uproperty_type>(3),{},[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::Benchmark))(); return nullptr; } }},
		{"GetNumber",{"GetNumber","int",static_cast<refl::store::uproperty_type>(5),{{static_cast<::refl::store::uproperty_type>(5),"int"},{static_cast<::refl::store::uproperty_type>(4),"bool"} },[](void* ptr, std::vector<void*> args) {int v = (*((TestScript*)ptr).*(&TestScript::GetNumber))(*(int*)args[0], *(bool*)args[1]); int* _ptr = new int(v); return (void*)_ptr; } }},
		{"GetInt2",{"GetInt2","int*",static_cast<refl::store::uproperty_type>(2),{},[](void* ptr, std::vector<void*> args) {int* v = (*((TestScript*)ptr).*(&TestScript::GetInt2))(); return (void*)v; } }},
		{"GetString",{"GetString","std::string",static_cast<refl::store::uproperty_type>(1),{},[](void* ptr, std::vector<void*> args) {std::string v = (*((TestScript*)ptr).*(&TestScript::GetString))(); std::string* _ptr = new std::string(v); return (void*)_ptr; } }},
		{"GetStruct",{"GetStruct","TestStruct",static_cast<refl::store::uproperty_type>(1),{},[](void* ptr, std::vector<void*> args) {TestStruct v = (*((TestScript*)ptr).*(&TestScript::GetStruct))(); TestStruct* _ptr = new TestStruct(v); return (void*)_ptr; } }},
		{"__TEST",{"__TEST","void",static_cast<refl::store::uproperty_type>(3),{{static_cast<::refl::store::uproperty_type>(1),"TestStruct"} },[](void* ptr, std::vector<void*> args) { (*((TestScript*)ptr).*(&TestScript::__TEST))(*(TestStruct*)args[0]); return nullptr; } }},
		{"GetStringPtr",{"GetStringPtr","std::string*",static_cast<refl::store::uproperty_type>(2),{},[](void* ptr, std::vector<void*> args) {std::string* v = (*((TestScript*)ptr).*(&TestScript::GetStringPtr))(); return (void*)v; } }}}});
	}
	inline static void Unload(::refl::store::storage* storage) {
		storage->discard("TestScript");
	}
};

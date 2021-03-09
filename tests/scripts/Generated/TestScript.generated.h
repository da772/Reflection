#pragma once
#include "reflection/reflection.hpp"
#include "../TestScript.h"

class TestScript_Generated : public refl::class_generation {
 	public:
	inline virtual void Load() override {
		refl::store::storage* storage = refl::reflector::Get()->GetStorage();
		storage->store("TestScript",{"TestScript",{
		{"int1",static_cast<refl::store::uproperty_type>(3),refl::store::offsetOf(&TestScript::int1) },
		{"int2",static_cast<refl::store::uproperty_type>(3),refl::store::offsetOf(&TestScript::int2) },
		{"numberptr",static_cast<refl::store::uproperty_type>(8),refl::store::offsetOf(&TestScript::numberptr) },
		{"char1",static_cast<refl::store::uproperty_type>(5),refl::store::offsetOf(&TestScript::char1) },
		{"uint1",static_cast<refl::store::uproperty_type>(4),refl::store::offsetOf(&TestScript::uint1) },
		{"uchar1",static_cast<refl::store::uproperty_type>(6),refl::store::offsetOf(&TestScript::uchar1) },
		{"uint64",static_cast<refl::store::uproperty_type>(16),refl::store::offsetOf(&TestScript::uint64) },
		{"bBool",static_cast<refl::store::uproperty_type>(2),refl::store::offsetOf(&TestScript::bBool) }}, {}});
	}
	inline virtual void Unload() override {
		refl::store::storage* storage = refl::reflector::Get()->GetStorage();
		storage->discard("TestScript");
	}
};
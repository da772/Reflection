#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>  

#include "utility.hpp"
#include "reflection/reflection.hpp"


//#include "scripts/Generated/TestScript.generated.h"

namespace refl {
	extern void LoadGeneratedFiles();
	extern void UnloadGeneratedFiles();
}

template<typename T>
T GetPtrVar(void* v, const std::string& clazz, const std::string& name) {
	const std::unordered_map<std::string, refl::store::uobject_struct>& map = refl::reflector::Get()->GetStorage()->get_map();
	uintptr_t o = map.at(clazz).property_map.at(name).offset;
	return *(T*)((uint8_t*)v + o);
}

template<typename T>
void SetPtrVar(void* v, const std::string& clazz, const std::string& name, T value) {
	const std::unordered_map<std::string, refl::store::uobject_struct>& map = refl::reflector::Get()->GetStorage()->get_map();
	uintptr_t o = map.at(clazz).property_map.at(name).offset;
	*(T*)((uint8_t*)v + o) = value;
}

template<typename T, typename ... Args >
T CallPtrFunction(void* v, const std::string& clazz, const std::string& name, Args&& ... args) {

}

int main() {
	refl::reflector* reflector = refl::reflector::Get();

	reflector->SetOutputDir((GetParentExecuteableDir(0)+std::string("tests/scripts/Generated/")).c_str());

	std::string in = GetParentExecuteableDir(0)+"tests/scripts/TestScript.h";
	std::ifstream t(in);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::cout << in << std::endl;
#if 0
	//std::string out = GetParentExecuteableDir(0)+"scripts/TestScript.generated.h";
	std::cout << in << std::endl;
	reflector->Generate(in.c_str());
	if (reflector->HasError()) {
		std::cout << reflector->GetError();
	}
	//std::cout << (out ? out : reflector->GetError()) << std::endl;

	// RELOAD DLL
	
#else
	refl::LoadGeneratedFiles();

	refl::store::storage* storage = reflector->GetStorage();

	const std::unordered_map<std::string,refl::store::uobject_struct>& map = storage->get_map();

	for (const std::pair<std::string, refl::store::uobject_struct>& p : map) {
		std::string s = p.first;
		for (const std::pair<std::string,refl::store::uproperty_struct>& _p : p.second.property_map) {
			s += " " + _p.second.name + ", "+_p.second.type_name + ", " + std::to_string(_p.second.offset) + " | ";
		}
		s += "\n";
		std::cout << s << std::endl;
	}

	void* v = map.at("TestScript").function_map.at("constructor").function(nullptr, {});
	uintptr_t o = map.at("TestScript").property_map.at("int2").offset;
	int *i = (int*)((uint8_t*)v + o);
	std::cout << std::to_string(*i) << std::endl;
	*(int*)((uint8_t*)v + o) = 51;
	std::cout << std::to_string(*i) << std::endl;
	int _i = 189;
	bool b = false;

	int* _f = (int*)map.at("TestScript").function_map.at("GetNumber").function(v, {(void*)&_i, (void*)&b});
	std::cout << std::to_string(*_f) << std::endl;
	free(_f);

	o = map.at("TestScript").property_map.at("numberptr").offset;
	i = *((int**)((uint8_t*)v + o));
	std::cout << std::to_string(*i) << std::endl;

	//int pp = GetPtrVar<int>(v, "TestScript", "int2", map);
	

	v = map.at("TestScript").function_map.at("~constructor").function(v, {});

	void* v1 = map.at("TestScript").function_map.at("constructor").function(nullptr, {});
	int pp = GetPtrVar<int>(v1, "TestScript", "int2");
	
	std::cout << std::to_string(pp) << std::endl;
	SetPtrVar<int>(v1, "TestScript", "int2", 23);
	pp = GetPtrVar<int>(v1, "TestScript", "int2");
	std::cout << std::to_string(pp) << std::endl;

	v1 = map.at("TestScript").function_map.at("~constructor").function(v1, {});
	
	refl::UnloadGeneratedFiles();
#endif
	refl::reflector::Destroy();
	return 0;
};

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>  
#include <type_traits>
#include <new>

#include "utility.hpp"
#include "reflection/reflection.hpp"

#include "scripts/TestScript.h"

#define ADDR(x) reinterpret_cast<uintptr_t>(x)


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


template <typename T, typename std::enable_if<!std::is_pointer<T>::value>::type* = nullptr>
T _CallPtrFunction(void* v, const std::string& clazz, const std::string& name, const std::vector<void*>& vec) {
	const std::unordered_map<std::string, refl::store::uobject_struct>& map = refl::reflector::Get()->GetStorage()->get_map();
	
	std::cout<< name << " NOT POINTER" << std::endl;
	T* _f = (T*)map.at(clazz).function_map.at(name).function(v, vec);
	T f = *_f;
	delete _f;
	return f;
}

template <typename T, typename std::enable_if<std::is_pointer<T>::value>::type* = nullptr>
T _CallPtrFunction(void* v, const std::string& clazz, const std::string& name, const std::vector<void*>& vec) {
	const std::unordered_map<std::string, refl::store::uobject_struct>& map = refl::reflector::Get()->GetStorage()->get_map();

	std::cout<< name << " POINTER" << std::endl;
	void* _f = map.at(clazz).function_map.at(name).function(v, vec);
	T f = (T)_f;
	return f;

}

template<typename T, typename ... Args>
T CallPtrFunction(void* v, const std::string& clazz, const std::string& name, Args&& ... args) {
	std::vector<void*> vec = {(void*)&args...};
	return _CallPtrFunction<T>(v, clazz, name, vec);
}

template<typename ... Args >
void CallPtrFunction(void* v, const std::string& clazz, const std::string& name, Args&& ... args) {
	const std::unordered_map<std::string, refl::store::uobject_struct>& map = refl::reflector::Get()->GetStorage()->get_map();
	std::vector<void*> vec = {(void*)&args...};
	void* f = map.at(clazz).function_map.at(name).function(v, vec);
	::refl::store::uproperty_type ret_type = map.at(clazz).function_map.at(name).ret_val;
	if (ret_type != ::refl::store::uproperty_type::_ptr && ret_type != ::refl::store::uproperty_type::uclass_ptr && ret_type != ::refl::store::uproperty_type::_void) {
		if (f != nullptr)
			free(f);
	}
	return;
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
		for (const std::pair<std::string,refl::store::ufunction_struct>& _p : p.second.function_map) {
			s+= _p.first +"\n";
		}
		std::cout << s << std::endl;
	}

	void* v1 = CallPtrFunction<void*>(v1, "TestScript", "constructor");
	
	int getNum = CallPtrFunction<int>(v1, "TestScript", "GetNumber", 823, false);
	int* getInt2 = CallPtrFunction<int*>(v1, "TestScript", "GetInt2");
	std::cout << "GET NUM = " << std::to_string(getNum) << std::endl;
	int _f = *getInt2;
	std::cout << "GET INT = " << std::to_string(_f) << std::endl;
	std::string getString = CallPtrFunction<std::string>(v1, "TestScript", "GetString");
	std::cout << getString << std::endl;
	std::string* getStringPtr = CallPtrFunction<std::string*>(v1, "TestScript", "GetStringPtr");
	std::cout << *getStringPtr << std::endl;
	*getStringPtr = "NEW STRING";
	getStringPtr = CallPtrFunction<std::string*>(v1, "TestScript", "GetStringPtr");
	std::cout << *getStringPtr << std::endl;
	void* getStruct = CallPtrFunction<void*>(v1, "TestScript", "GetStruct");
	TestStruct* ts = (TestStruct*)getStruct;
	std::cout << ts->i << std::endl;
	delete ts;
	
	CallPtrFunction(v1, "TestScript", "~constructor");
	v1 = nullptr;
	//v1 = map.at("TestScript").function_map.at("~constructor").function(v1, {});
	
	refl::UnloadGeneratedFiles();
#endif
	refl::reflector::Destroy();
	return 0;
};

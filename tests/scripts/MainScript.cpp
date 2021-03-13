#include "MainScript.h"

#include <vector>
#include <iostream>


void MainScript::Benchmark() {

	refl::store::storage* storage = reflect->GetStorage();
	//reflect.LoadGeneratedFiles();

	const std::unordered_map<std::string, refl::store::uobject_struct>& map = storage->get_map();
	std::cout << "PRINTING MAP : " << map.size() << std::endl;

	for (const std::pair<std::string, refl::store::uobject_struct>& p : map) {
		std::string s = p.first;
		for (const std::pair<std::string, refl::store::uproperty_struct>& _p : p.second.property_map) {
			s += " " + _p.second.name + ", " + _p.second.type_name + ", " + std::to_string(_p.second.offset) + " | ";
		}
		s += "\n";
		for (const std::pair<std::string, refl::store::ufunction_struct>& _p : p.second.function_map) {
			s += _p.first + "\n";
		}
		std::cout << s << std::endl;
	}
	std::cout << "MAP PRINTED" << std::endl;
	double ns = 0;
	double n = 0;
	{
		uint64_t time = GetTimeNS();
		refl::uClass uClss = reflect->CreateUClass("TestScript");
		std::cout << "HERE" << std::endl;
		//void* v1 = uClss.data();

		int getNum = uClss.CallFunction<int>("GetNumber", 823, false); 
		int* getInt2 = uClss.CallFunction<int*>("GetInt2");
		std::vector<int> vec = uClss.GetMember<std::vector<int>>("vec");
		for (int i = 0; i < vec.size(); i++) {
			std::cout << "VEC["<<i<<"]= " << vec[i] << std::endl;
		}
		const int& __iptr = uClss.GetMember<int>("int2");
		
		*getInt2 = 55;

		std::cout << __iptr << std::endl;

		std::string getString = uClss.CallFunction<std::string>("GetString");
		std::string* getStringPtr = uClss.CallFunction<std::string*>("GetStringPtr");
		*getStringPtr = "NEW STRING";
		getStringPtr = uClss.CallFunction<std::string*>("GetStringPtr");

		refl::uClass exampleScript = uClss.CallFunction<refl::uClass>("GetExampleScript");
		std::cout << "UCLASS RETREIEVED" << std::endl;
		std::string n = exampleScript.GetMember<std::string>("name");
		uint32_t id = exampleScript.GetMember<uint32_t>("id");
		std::cout <<"Example Script: " << id << " name: " << n << std::endl;
		uClss.CallFunction<void>("Begin");
		void* v = uClss.CallFunction<void*>("GetVoidPtr");
		std::cout << reinterpret_cast<uintptr_t>(v) << std::endl;

		int iRefCheck = 0;
		int& iRef = uClss.CallFunction<int&>("GetIntRef", iRefCheck);
		iRef = 18832;
		std::cout << iRefCheck << " " << *getInt2 << std::endl;

		time = GetTimeNS() - time;
		//std::cout << "\n\n" << (double)time / 1e6 << " ms | " << time << " ns\n\n" << std::endl;
		ns += (double)time / 1e6;
	}
	std::cout << "REFLECTION: " << ns  << "ms" << std::endl;

    std::cout << "HELLO WORLD4123" << std::endl;

	//std::cout << (ns < n ? "REFLECTION " : "NATIVE ") << " is " << (ns < n ? n / ns : ns / n) << "x faster" << std::endl;

	//reflect.UnloadGeneratedFiles();
}

void MainScript::Run() {
    Benchmark();
    
    while (true) {
        std::cout << "Enter Command: ";
		char userInput;
		std::cin >> userInput;
		if (userInput == 'e') {
            Benchmark();
		}
		if (userInput == 'r') {
            exitCode = 1;
			break;
		}
        if (userInput == 'q') {
            exitCode = 0;
            break;
        }
    }
    
}
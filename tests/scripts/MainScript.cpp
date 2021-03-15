#include "MainScript.h"

#include <vector>
#include <iostream>

#include "TestScript.h"

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
	uint64_t ns = 0;
	uint64_t n = 0;
	uint32_t amt = 1;
	for (uint32_t i = 0 ; i < amt; i++)
	{
		try 
		{
			uint64_t time = GetTimeNS();
			refl::uClass uClss = reflect->CreateUClass("TestScript");
			int getNum = uClss.CallFunction<int>("GetNumber", 823, false); 
			int* getInt2 = uClss.CallFunction<int*>("GetInt2");
			std::vector<int> vec = uClss.GetMember<std::vector<int>>("vec");
			const int& __iptr = uClss.GetMember<int>("int2");
			*getInt2 = 55;
			std::string getString = uClss.CallFunction<std::string>("GetString");
			std::string* getStringPtr = uClss.CallFunction<std::string*>("GetStringPtr");
			*getStringPtr = "NEW STRING";
			getStringPtr = uClss.CallFunction<std::string*>("GetStringPtr");
			refl::uClass exampleScript = uClss.CallFunction<refl::uClass>("GetExampleScript");
			std::string n = exampleScript.GetMember<std::string>("name");
			uint32_t id = exampleScript.GetMember<uint32_t>("id");
			uClss.CallFunction<void>("Begin");
			void* v = uClss.CallFunction<void*>("GetVoidPtr");
			int iRefCheck = 0;
			int& iRef = uClss.CallFunction<int&>("GetIntRef", iRefCheck);
			iRef = 18832;
			time = GetTimeNS() - time;
			ns += time;
			uClss.CallFunction<void>("Error!!"); // Catch this error

		} 
		catch (std::exception& e) 
		{
			std::cout << "REFLECTION ERROR: " <<  e.what() << std::endl;
		}

		try 
		{
			uint64_t time = GetTimeNS();
			TestScript* testScript = new TestScript();
			int getNum = testScript->GetNumber(832, false);
			int* getInt2 = testScript->GetInt2();
			std::vector<int> vec = testScript->vec;
			const int& _iptr = testScript->int2;
			*getInt2 = 55;
			std::string getString = testScript->GetString();
			std::string* getStringPtr = testScript->GetStringPtr();
			*getStringPtr = "NEW STRING";
			getStringPtr = testScript->GetStringPtr();
			ExampleScript* exampleScript = testScript->GetExampleScript();
			std::string _n = exampleScript->name;
			uint32_t id = exampleScript->id;
			testScript->Begin();
			void* v = testScript->GetVoidPtr();
			int iRefCheck = 0;
			int& iRef = testScript->GetIntRef(iRefCheck);
			iRef = 18832;
			delete testScript;
			time = GetTimeNS() - time;
			n += time;
			// delete exampleScript; Cant catch delete errors
		} catch (std::exception& e) {
			std::cout << "NASDATIVE ERROR: " << e.what() << std::endl;
		}
	}

	{
		uint64_t time = GetTimeNS();
		refl::uClass uClss = reflect->CreateUClass("TestScript");
		uClss.CallFunction<void>("Benchmark");
		time = GetTimeNS() - time;
		ns += time;
	}

	{
		uint64_t time = GetTimeNS();
		TestScript* testScript = new TestScript();
		testScript->Benchmark();
		time = GetTimeNS() - time;
		n += time;
		delete testScript;
	}
    
	//ns /= amt;
	//n /= amt;
	std::cout << "REFLECTION: " << ns << " ns | NATIVE: " << n << " ns" << std::endl;
	std::cout << (ns < n ? "REFLECTION " : "NATIVE ") << " is " << (ns < n ? n / ns : ns / n) << "x faster" << std::endl;

	//reflect.UnloadGeneratedFiles();
}

void MainScript::Run() {
    while (true) {
		exitCode = 0;
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
		if (userInput == 'g') {
			reflect->Clear();
			for (const std::string& s : *scriptFiles) {
				std::cout << "Generating: " << s << std::endl;
				__GenerateLib((files::GetParentExecuteableDir(3) + "tests/scripts/"), s, *reflect);
			}
		}
    }
    
}
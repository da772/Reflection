#include "MainScript.h"

#include <vector>
#include <iostream>
#include <assert.h>

#include "TestScript.h"

void MainScript::Benchmark() {

	refl::store::storage* storage = reflect->GetStorage();
	//reflect.LoadGeneratedFiles();

	const std::unordered_map<std::string, refl::store::uobject_struct>& map = storage->get_map();
	std::cout << "PRINTING MAP : " << map.size() << std::endl;

	try {
	std::string myStr = "YES";
	std::string r = reflect->CallStaticFunction<const std::string&>("TestScript", "StaticFunc",55, myStr);

	std::cout << "STATIC FUNC: " << r << std::endl;
	} catch (std::exception& e) {
		std::cout << "EXCEPTION: " << e.what() << std::endl;
	}

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


	std::cout << "\nRunning Test 1... " << std::endl;
	try 
	{
		uint64_t time = GetTimeNS();
		refl::uClass uClss = reflect->CreateUClass("TestScript");
		int getNum = uClss.CallFunction<int>("GetNumber", 823, false);
		assert(getNum == -823);
		
		int* getInt2 = uClss.CallFunction<int*>("GetInt2");
		assert(*getInt2 == -20);
		refl::uClass _cls = uClss.GetMember<refl::uClass>("classTwo");
		assert(_cls.GetMember<int>("i") == 25);
		_cls.GetMember<int>("i") = 50;
		assert(_cls.GetMember<int>("i") == 50);
		_cls.GetMember<int>("i") = 33;
		assert(_cls.GetMember<int>("i") == 33);
		std::vector<int> vec = uClss.GetMember<std::vector<int>>("vec");
		assert(vec == std::vector<int>({1}) );
		const int& __iptr = uClss.GetMember<int>("int2");
		assert(__iptr == -20);
		*getInt2 = 55;
		assert(__iptr == 55);
		std::string getString = uClss.CallFunction<std::string>("GetString");
		assert(getString == "MyString");
		std::string* getStringPtr = uClss.CallFunction<std::string*>("GetStringPtr");
		assert(*getStringPtr == "Hello World!");
		*getStringPtr = "NEW STRING";
		getStringPtr = uClss.CallFunction<std::string*>("GetStringPtr");
		assert(*getStringPtr == "NEW STRING");
		refl::uClass exampleScript = uClss.CallFunction<refl::uClass>("GetExampleScript");
		std::string n = exampleScript.GetMember<std::string>("name");
		assert(n == "My Script");
		uint32_t id = exampleScript.GetMember<uint32_t>("id");
		assert(id == 42069);
		uClss.CallFunction<void>("Begin");
		void* v = uClss.CallFunction<void*>("GetVoidPtr");
		assert((*(bool*)v) == false);
		int iRefCheck = 0;
		int& iRef = uClss.CallFunction<int&>("GetIntRef", iRefCheck);
		assert(iRefCheck == 10);
		iRef = 18832;
		assert(18832 ==  uClss.CallFunction<int&>("GetIntRef", iRefCheck));

		char** ptrPtr = uClss.GetMember<char**>("doublePtr");
		for (int i =0 ; i < 5; i++) {
			std::cout << ptrPtr[i] << std::endl;
		}


		int staticInt = reflect->CallStaticFunction<int>("StaticClass","StaticFunction");
		assert(0xDEAD == staticInt);
		
		std::cout << "Test 1 Passed" << std::endl;
	} 
	catch (std::exception& e) 
	{
		std::cout << "REFLECTION ERROR: " <<  e.what() << std::endl;
		return;
	}

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
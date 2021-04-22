#pragma once
#include "NativeScript.h"
#include "Entity.hpp"

#include "ExampleScript.h"

#include <vector>
#include <iostream>
#include <cstring>


struct TestStruct {
    int i = 69;
};

UCLASS()
class ClassTwo {
    public:
    UCONSTRUCTOR()
    inline ClassTwo(int i) : i(i) {};
    inline ~ClassTwo() {};

    UFUNCTION()
    inline void TestFunc() {};

    UPROPERTY() 
    int i = 12;

    UPROPERTY()
    int testProperty = 5;
};

UCLASS()
class TestScript : public NativeScript {
    UCLASS_BODY()
    public:

    UCONSTRUCTOR()
    inline TestScript() {  
        
        doublePtr = (char**)malloc(sizeof(char*)*5);
        for (int i = 0; i < 5; i++) {
            std::string str = "TEST STRING: " + std::to_string(i);
            doublePtr[i] = (char*)calloc( str.size()+1,sizeof(char));
            memcpy(doublePtr[i], str.data(), str.size());
        }

    };
    inline ~TestScript() { 
        for (int i = 0; i < 5; i++) {
            free(doublePtr[i]);
        }
        free(doublePtr);

    };

    UFUNCTION()
    virtual inline void Update() override {};
    UFUNCTION()
    virtual inline void End() override {};

    UPROPERTY()
    int int1 = Entity::TestFunction();
    UPROPERTY()
    int int2 = -20;
    UPROPERTY()
    int *numberptr = &int1;
    UPROPERTY()
    char char1 = 'a';
    UPROPERTY()
    unsigned int uint1 = 15; 

    UPROPERTY()
    unsigned char uchar1 = 'c';

    UPROPERTY()
    uint64_t uint64 = 222;

    UPROPERTY()
    std::vector<int> vec = {1};

    UPROPERTY()
    bool bBool = false;

    UPROPERTY()
    TestStruct myStruct; 

    UFUNCTION()
    virtual inline void Begin() override { };

    UFUNCTION()
    Entity* GetEntity() { return new Entity();}

    ExampleScript exampleScript = ExampleScript();

    UFUNCTION()
    inline ExampleScript* GetExampleScript() {
        return &exampleScript;
    }

    UFUNCTION()
    inline void* GetVoidPtr() {
        return (void*)&bBool;
    }

    UFUNCTION()
    inline int& GetIntRef(int& r) {
        r = 10;
        return int2;
    }

    UFUNCTION()
    void Benchmark();


    uint64_t benchMarkInt = 0;

    UFUNCTION()
    int GetNumber(int i, bool b);

    UFUNCTION()
    inline int* GetInt2() {
        return &this->int2;
    }

    UFUNCTION()
    inline std::string GetString() {
        //std::cout <<"GETTING STRING" << std::endl;
        return "MyString";
    }

    UFUNCTION()
    inline TestStruct GetStruct() {
        return TestStruct();
    }


	UFUNCTION()
	inline void __TEST(TestStruct t) {};

/*
    inline TestScript GetScript() {
        return TestScript();
    }
*/

    UPROPERTY()
    char** doublePtr = nullptr;

    UPROPERTY()
    ClassTwo classTwo = ClassTwo(25);

    UFUNCTION()
    static inline std::string& StaticFunc(int i, std::string& s) {std::cout << "STATIC FUNC " << s << std::endl; s = "POOP"; return s;}

    UPROPERTY()
    std::string myString = "Hello World!";

    UFUNCTION()
    inline std::string* GetStringPtr() {
        return &myString;
    }

};


UCLASS()
class AnothaClass {
    public:
    UCONSTRUCTOR()
    inline AnothaClass() {}
    inline ~AnothaClass() {}

    UFUNCTION()
    inline int AnotheFunction() {return 0;}

    UPROPERTY()
    std::vector<int> myArr;

};
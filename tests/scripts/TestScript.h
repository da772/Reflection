#pragma once
#include "NativeScript.h"
#include "Entity.hpp"

#include "ExampleScript.h"

#include <vector>
#include <iostream>


struct TestStruct {
    int i = 69;
};

UCLASS()
class TestScript : public NativeScript {
    UCLASS_BODY()
    public:

    UCONSTRUCTOR()
    inline TestScript() {  };
    inline ~TestScript() {  };

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

    UFUNCTION()
    static inline std::string& StaticFunc(int i, std::string& s) {std::cout << "STATIC FUNC " << s << std::endl; s = "POOP"; return s;}

    UPROPERTY()
    std::string myString = "Hello World!";

    UFUNCTION()
    inline std::string* GetStringPtr() {
        return &myString;
    }

};

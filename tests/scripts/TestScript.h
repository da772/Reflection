#pragma once
#include "NativeScript.h"
#include "Entity.hpp"

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
    inline ~TestScript() { };

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
    std::vector<int> vec = {0, 1 , 2};

    UPROPERTY()
    bool bBool = false;

    UPROPERTY()
    TestStruct myStruct; 

    UFUNCTION()
    virtual inline void Begin() override {};

    UFUNCTION()
    Entity* GetEntity() { return new Entity();}

    UFUNCTION()
    inline int GetNumber(int i, bool b) { 
       // std::cout<<"GET NUMBER CALLED " << std::to_string(i) << " " << std::to_string(b) << std::endl;
        std::cout << "ROMA IS HOT" << std::endl;
        if (b) return i; 
        else return -i;
         
        };

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

/*
    inline TestScript GetScript() {
        return TestScript();
    }
*/
    UPROPERTY()
    std::string myString = "Hello World!";

    UFUNCTION()
    inline std::string* GetStringPtr() {
        return &myString;
    }

};

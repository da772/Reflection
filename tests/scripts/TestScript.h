#pragma once
#include "NativeScript.h"
#include "Entity.hpp"

#include <iostream>

struct TestStruct {
    int i = 0;
};

UCLASS()
class TestScript : public NativeScript {
    UCLASS_BODY()
    public:
    UFUNCTION()
    inline TestScript() { std::cout << "TEST SCRIPT CREATED" << std::endl; };
    inline ~TestScript() { std::cout << "TEST SCRIPT DESTROYED" << std::endl;};

   
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
    inline Entity* GetEntity() { return new Entity();}

    UFUNCTION()
    inline int GetNumber(int i, bool b) { if (b) return i; else return -i;};

};

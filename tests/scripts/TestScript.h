#pragma once
#include "NativeScript.h"
#include "reflection/reflection.hpp"
#include "reflection/macros.h"

UCLASS()
class TestScript : public NativeScript {
    UCLASS_BODY()
    public:
        TestScript();
        ~TestScript();

   
    UFUNCTION()
    virtual void Update() override {};
    UFUNCTION()
    virtual void End() override {};

    UPROPERTY()
    int int1 = 10;
    UPROPERTY()
    int int2 = -20;
    UPROPERTY()
    int *numberptr = 0;
    UPROPERTY()
    char char1 = 'a';
    UPROPERTY()
    unsigned int uint1 = 15; 
    UPROPERTY()
    unsigned char uchar1 = 'c';
    UPROPERTY()
    uint64_t uint64 = 222;
    UPROPERTY()
    bool bBool = false;

     UFUNCTION()
    virtual void Begin() override {
        
    };



};

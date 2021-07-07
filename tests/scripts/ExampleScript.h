#pragma once

#include "NativeScript.h"
#include <string>
#include <cstdint>
#include <iostream>


UCLASS()
class MostBasedClass {
public:
	UFUNCTION()
		inline int MostBasedFunc() { return 0xBA5ED*2; }

	UPROPERTY()
		int mostBasedProperty = 0xBA5ED * 3;

};

UCLASS()
class MoreBasedClass : public MostBasedClass{
public:
	UFUNCTION()
		inline int BasedFunc() { return 0xBA5ED; }

	UPROPERTY()
		int basedProperty = 0xBA5ED * 2;

};

UCLASS()
class BaseClass : public MoreBasedClass {
public:
	UCONSTRUCTOR()
		inline BaseClass() {};

	inline ~BaseClass() {};

	UFUNCTION()
    inline int ParentFunc() { return 0xDAD; };

	UPROPERTY()
	int parentProperty = 0xD3AD;
};

UCLASS()
class ExampleScript : public NativeScript {
    UCLASS_BODY()
    public:
    UCONSTRUCTOR()
    inline ExampleScript() {};
    inline ~ExampleScript() {};

    UPROPERTY()
    std::string name = "My Script";
    UPROPERTY()
    uint32_t id = 42069;



};
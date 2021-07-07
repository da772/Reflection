#pragma once

#include "NativeScript.h"


UCLASS()
class StaticClass {

public:
	UFUNCTION()
	inline static int StaticFunction() {
		return 0xDEAD;
	}

	UPROPERTY()
	int r;

};
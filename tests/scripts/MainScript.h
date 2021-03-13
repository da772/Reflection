#pragma once
#include "NativeScript.h"

#include "reflection/reflection.hpp"
#include "utility.hpp"

UCLASS()
class MainScript : public NativeScript {
    UCLASS_BODY()
    public:

    UCONSTRUCTOR()
    inline MainScript() {  };
    inline ~MainScript() {  };

    UFUNCTION()
    void Run();

    void Benchmark();

    UPROPERTY()
    refl::reflector* reflect;

    UPROPERTY()
    dllptr lib;

    UPROPERTY()
    int exitCode = 0;

};

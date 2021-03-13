#pragma once
#include "NativeScript.h"

#include "reflection/reflection.hpp"
#include "utility.hpp"

#include <vector>

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
    int m;

    UPROPERTY()
    dllptr lib;

    UPROPERTY()
    std::vector<std::string>* scriptFiles;

    UPROPERTY()
    int exitCode = 0;

};

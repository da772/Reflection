#pragma once

#include "NativeScript.h"
#include <string>
#include <cstdint>

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
    uint32_t id = 23123;



};
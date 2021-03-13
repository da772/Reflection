#include "TestScript.h"

int TestScript::GetNumber(int i, bool b) { 
    // std::cout<<"GET NUMBER CALLED " << std::to_string(i) << " " << std::to_string(b) << std::endl;
    std::cout << "RELOAD" << std::endl;
    ExampleScript exampleScript = ExampleScript();
    std::cout << "ExampleScript Name: \"" << exampleScript.name << "\" ID: " << exampleScript.id << std::endl;
    if (b) return i; 
    else return -i;       
};

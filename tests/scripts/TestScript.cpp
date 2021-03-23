#include "TestScript.h"


#include <math.h>


int TestScript::GetNumber(int i, bool b) { 
    if (b) return i; 
    else return -i;       
};

void TestScript::Benchmark() {
    for (int i = 0; i < 1e6; i++) {
        benchMarkInt += sqrt(i);
    }
}
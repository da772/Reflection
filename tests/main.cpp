#include <iostream>
#include "reflection/reflection.hpp"

int main() {
	refl::reflector reflector;
	std::cout << "Hello World! : " << reflector.GetError()  << std::endl;
	return 0;
}

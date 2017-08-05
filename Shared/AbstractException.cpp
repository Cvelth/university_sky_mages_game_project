#include "AbstractException.hpp"
#include <iostream>

#define ClassName(name) #name

void Exceptions::AbstractException::print() const {
	std::cerr << ClassName(this) << " was thrown.\n";
	std::cerr << what() << '\n';
}
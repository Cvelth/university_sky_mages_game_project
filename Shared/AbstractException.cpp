#include "AbstractException.hpp"
#include <iostream>

void Exceptions::AbstractException::print() const {
	std::cerr << getName() << " was thrown.\n";
	std::cerr << what() << '\n';
}
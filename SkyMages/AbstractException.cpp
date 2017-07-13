#include "AbstractExceptions.hpp"
#include <iostream>

void Exceptions::AbstractException::print() {
	std::cerr << "An Exception was thrown..." << std::endl;
}

void Exceptions::AbstractStringException::print() {
	std::cerr << name << " was thrown.\n";
	std::cerr << desc << std::endl;
}

void Exceptions::AbstractIntegerErrorException::print() {
	std::cerr << name << " was thrown with code " << error_code << std::endl;
	std::cerr << desc << std::endl;
}

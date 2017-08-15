#pragma once
#include "Objects\AbstractObjects\IndependentObject.hpp"

class Actor : public IndependentObject {
public:
	using IndependentObject::IndependentObject;
	~Actor() {}
};
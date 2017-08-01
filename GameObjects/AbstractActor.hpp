#pragma once
#include "AbstractGameObject.hpp"

class AbstractActor : public AbstractGameObject {
public:
	using AbstractGameObject::AbstractGameObject;
	~AbstractActor();
};

class ControllableActor : public AbstractActor {
public:
	using AbstractActor::AbstractActor;
	~ControllableActor();
};

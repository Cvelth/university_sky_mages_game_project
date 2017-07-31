#pragma once
#include "Events\EmptyEventHandler.hpp"

class GameController : public mgl::EmptyEventHandler {
public:
	GameController();
	~GameController();
};


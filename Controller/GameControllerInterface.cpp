#include "GameControllerInterface.hpp"
#include "GameController.hpp"

GameControllerInterface::GameControllerInterface() {
	m_controller = new GameController();
}

GameControllerInterface::~GameControllerInterface() {
	delete m_controller;
}
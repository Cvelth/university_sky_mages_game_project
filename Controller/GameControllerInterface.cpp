#include "GameControllerInterface.hpp"
#include "GameController.hpp"

GameControllerInterface::GameControllerInterface() {
	m_controller = new GameController();
}

GameControllerInterface::~GameControllerInterface() {
	delete m_controller;
}

void GameControllerInterface::startCameraControl(GameCamera* camera) {
	m_controller->startCameraControl(camera);
}

void GameControllerInterface::stopCameraControl() {
	m_controller->stopCameraControl();
}

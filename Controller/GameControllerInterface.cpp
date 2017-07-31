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

void GameControllerInterface::setMainActor(ControllableActor* actor) {
	m_controller->setMainActor(actor);
}
void GameControllerInterface::removeMainActor() {
	m_controller->removeMainActor();
}
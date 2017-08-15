#include "GameControllerInterface.hpp"
#include "GameController.hpp"

GameControllerInterface::GameControllerInterface() {
	m_controller = new GameController();
}

GameControllerInterface::~GameControllerInterface() {
	delete m_controller;
}

void GameControllerInterface::startCameraControl(Camera* camera) {
	m_controller->startCameraControl(camera);
}

void GameControllerInterface::stopCameraControl() {
	m_controller->stopCameraControl();
}

void GameControllerInterface::setMainActor(MainActor* actor) {
	m_controller->setMainActor(actor);
}
void GameControllerInterface::removeMainActor() {
	m_controller->removeMainActor();
}

void GameControllerInterface::startKeyControl(KeyLayout const *keys) {
	m_controller->startKeyControl(keys);
}
void GameControllerInterface::stopKeyControl() {
	m_controller->stopKeyControl();
}

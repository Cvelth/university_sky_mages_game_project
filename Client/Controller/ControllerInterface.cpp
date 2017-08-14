#include "ControllerInterface.hpp"
#include "Controller.hpp"

ControllerInterface::ControllerInterface() {
	m_controller = new Controller();
}

ControllerInterface::~ControllerInterface() {
	delete m_controller;
}

void ControllerInterface::startCameraControl(Camera* camera) {
	m_controller->startCameraControl(camera);
}

void ControllerInterface::stopCameraControl() {
	m_controller->stopCameraControl();
}

void ControllerInterface::setMainActor(MainActor* actor) {
	m_controller->setMainActor(actor);
}
void ControllerInterface::removeMainActor() {
	m_controller->removeMainActor();
}

void ControllerInterface::startKeyControl(KeyLayout const *keys) {
	m_controller->startKeyControl(keys);
}
void ControllerInterface::stopKeyControl() {
	m_controller->stopKeyControl();
}

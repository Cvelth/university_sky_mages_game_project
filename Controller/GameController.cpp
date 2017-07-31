#include "GameController.hpp"
#include "GameLogicEngine\GameCamera.hpp"

GameController::GameController() : m_controlledCamera(nullptr) {}

GameController::~GameController() {}

void GameController::mouseScrollEvent(GLFWwindow* w, double x, double y) {
	if (m_controlledCamera)
		m_controlledCamera->changeZoom(1.0 + float(y) / 10.0);
}

void GameController::startCameraControl(GameCamera* camera) {
	m_controlledCamera = camera;
}

void GameController::stopCameraControl() {
	m_controlledCamera = nullptr;
}

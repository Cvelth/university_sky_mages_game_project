#include "GameController.hpp"
#include "GameLogicEngine\GameCamera.hpp"
#include "GameObjects\AbstractActor.hpp"

GameController::GameController() : m_controlledCamera(nullptr) {}

GameController::~GameController() {}

void GameController::mouseScrollEvent(GLFWwindow* w, double x, double y) {
	if (m_controlledCamera)
		m_controlledCamera->changeZoom(1.f + float(y) / 10.f);
}

void GameController::startCameraControl(GameCamera* camera) {
	m_controlledCamera = camera;
}

void GameController::stopCameraControl() {
	m_controlledCamera = nullptr;
}

void GameController::setMainActor(ControllableActor* actor) {
	m_actor = actor;
}
void GameController::removeMainActor() {
	m_actor = nullptr;
}
#include "GameController.hpp"
#include "GameLogicEngine\GameCamera.hpp"
#include "GameObjects\AbstractActor.hpp"

GameController::GameController() : m_controlledCamera(nullptr) {}

GameController::~GameController() {}

void GameController::mouseScrollEvent(GLFWwindow* w, double x, double y) {
	if (m_controlledCamera)
		m_controlledCamera->changeZoom(1.f + float(y) / 10.f);
}

void GameController::resizeEvent(GLFWwindow *w, int x, int y) {
	if (m_controlledCamera) {
		auto newAR = float(x) / y;
		float mgn = newAR / m_controlledCamera->aspectRatio();
		m_controlledCamera->changeZoom(mgn);
		m_controlledCamera->changeAspectRatio(newAR);
	}
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
#include "GameController.hpp"
#include "LogicEngine\GameCamera.hpp"
#include "GameObjects\AbstractActors.hpp"
#include "Settings\KeyLayout.hpp"

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


void GameController::keyEvent(GLFWwindow *w, mgl::Key key, int scancode, mgl::KeyAction action, mgl::Mods mods) {
	if (m_keys) {
		if (key == *m_keys->move_up && action == mgl::KeyAction::press) {
			return;
		} else if (key == *m_keys->move_down && action == mgl::KeyAction::press) {
			return;
		} else if (key == *m_keys->move_left && action == mgl::KeyAction::press) {
			return;
		} else if (key == *m_keys->move_right && action == mgl::KeyAction::press) {
			return;
		}
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

void GameController::startKeyControl(KeyLayout const *keys) {
	m_keys = keys;
}

void GameController::stopKeyControl() {
	m_keys = nullptr;
}

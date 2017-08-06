#include "GameController.hpp"
#include "LogicEngine\GameCamera.hpp"
#include "GameObjects\MainActor.hpp"
#include "GameObjects\AbstractEquipableItems.hpp"
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
		if (action == mgl::KeyAction::press) {
			if (key == *m_keys->move_up)
				m_actor->m_engine->accelerate_up();
			else if (key == *m_keys->move_down)
				m_actor->m_engine->accelerate_down();
			else if (key == *m_keys->move_left)
				m_actor->m_engine->accelerate_left();
			else if (key == *m_keys->move_right)
				m_actor->m_engine->accelerate_right();
			else if (key == *m_keys->anti_gravity)
				if (m_actor->m_engine->is_auti_gravity_turned_on())
					m_actor->m_engine->turn_off_anti_gravity();
				else
					m_actor->m_engine->turn_on_anti_gravity(m_actor->mass());
		} else if (action == mgl::KeyAction::release) {
			if (key == *m_keys->move_up)
				m_actor->m_engine->stopAcceleration_v();
			else if (key == *m_keys->move_down)
				m_actor->m_engine->stopAcceleration_v();
			else if (key == *m_keys->move_left)
				m_actor->m_engine->stopAcceleration_h();
			else if (key == *m_keys->move_right)
				m_actor->m_engine->stopAcceleration_h();
		}
	}
}

void GameController::startCameraControl(GameCamera* camera) {
	m_controlledCamera = camera;
}

void GameController::stopCameraControl() {
	m_controlledCamera = nullptr;
}

void GameController::setMainActor(MainActor* actor) {
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

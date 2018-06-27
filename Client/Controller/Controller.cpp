#include "Controller.hpp"
#include "Engines/Camera/Camera.hpp"
#include "Objects/Actors/MainActor.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Shared/KeyLayout.hpp"
#include "Shared/GameMode.hpp"

Controller::Controller() : m_controlledCamera(nullptr) {}

Controller::~Controller() {}


void Controller::mouseScrollEvent(GLFWwindow* w, double x, double y) {
	if (m_controlledCamera)
		m_controlledCamera->changeZoom(1.f + float(y) / 10.f);
}

void Controller::resizeEvent(GLFWwindow *w, int x, int y) {
	current_screen_width = x;
	current_screen_height = y;
	if (m_controlledCamera) {
		auto newAR = float(x) / y;
		float mgn = newAR / m_controlledCamera->aspectRatio();
		m_controlledCamera->changeZoom(mgn);
		m_controlledCamera->changeAspectRatio(newAR);
	}
}

void Controller::keyEvent(GLFWwindow *w, mgl::Key key, int scancode, mgl::KeyAction action, mgl::Mods mods) {
	if (m_keys) {
		if (action == mgl::KeyAction::press) {
			if (key == *m_keys->move_up)
				m_actor->m_engine->accelerate_up(false);
			else if (key == *m_keys->move_down)
				m_actor->m_engine->accelerate_down(false);
			else if (key == *m_keys->move_left)
				m_actor->m_engine->accelerate_left(false);
			else if (key == *m_keys->move_right)
				m_actor->m_engine->accelerate_right(false);
			else if (key == *m_keys->anti_gravity)
				if (m_actor->m_engine->is_auti_gravity_turned_on())
					m_actor->m_engine->turn_off_anti_gravity();
				else
					m_actor->m_engine->turn_on_anti_gravity(m_actor->mass());
			else if (key == *m_keys->pause)
				GameModeController::pauseGame();
		} else if (action == mgl::KeyAction::release) {
			if (key == *m_keys->move_up)
				m_actor->m_engine->accelerate_up(true);
			else if (key == *m_keys->move_down)
				m_actor->m_engine->accelerate_down(true);
			else if (key == *m_keys->move_left)
				m_actor->m_engine->accelerate_left(true);
			else if (key == *m_keys->move_right)
				m_actor->m_engine->accelerate_right(true);
		}
	}
}
void Controller::mouseButtonEvent(GLFWwindow *w, mgl::MouseButton button, mgl::MouseAction action, mgl::Mods mods) {
	if (action == mgl::MouseAction::press) {
		if (button == mgl::MouseButton::left)
			m_actor->activateRightWeapon();
		else if (button == mgl::MouseButton::right)
			m_actor->activateLeftWeapon();
	} else if (action == mgl::MouseAction::release)
		if (button == mgl::MouseButton::left)
			m_actor->deactivateRightWeapon();
		else
			m_actor->deactivateLeftWeapon();
}

void Controller::startCameraControl(Camera* camera) {
	m_controlledCamera = camera;
}

void Controller::stopCameraControl() {
	m_controlledCamera = nullptr;
}

void Controller::setMainActor(MainActor* actor) {
	m_actor = actor;
}
void Controller::removeMainActor() {
	m_actor = nullptr;
}

void Controller::startKeyControl(KeyLayout const *keys) {
	m_keys = keys;
}

void Controller::stopKeyControl() {
	m_keys = nullptr;
}

#include "../MyGraphicsLibrary/MGL/OpenGL/FunctionsMirror/ExperimentalFunctionsMirror.hpp"
void Controller::mouseMoveEvent(GLFWwindow *w, double x, double y) {
	if (!current_screen_width || !current_screen_height)
		mgl::getWindowSize(w, &current_screen_width, &current_screen_height);
	auto temp_x = m_controlledCamera->minX();
	auto temp_y = m_controlledCamera->minY();
	float tx = float(x) / current_screen_width * (m_controlledCamera->maxX() - temp_x) + temp_x;
	float ty = float(y) / current_screen_height * (m_controlledCamera->maxY() - temp_y) + temp_y;
	m_actor->aim(tx, ty);
}
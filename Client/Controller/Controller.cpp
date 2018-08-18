#include "Controller.hpp"
#include "Camera.hpp"
#include "Objects/Actors/MainActor.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Shared/KeyLayout.hpp"
#include "Shared/GameStateController.hpp"

Controller::Controller() : m_controlledCamera(nullptr) {}
Controller::~Controller() {}

void Controller::mouseScrollEvent(GLFWwindow* w, double x, double y) {
	if (m_controlledCamera)
		m_controlledCamera->changeZoom(1.f - float(y) / 10.f);
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

void Controller::startCameraControl(std::shared_ptr<Camera> camera) {
	m_controlledCamera = camera;
}
void Controller::stopCameraControl() {
	m_controlledCamera = nullptr;
}
void Controller::setMainActor(std::shared_ptr<MainActor> actor) {
	m_actor = actor;
}void Controller::removeMainActor() {
	m_actor = nullptr;
}
void Controller::startKeyControl(KeyLayout const *keys) {
	m_keys = keys;
}
void Controller::stopKeyControl() {
	m_keys = nullptr;
}

#include "Engines/Networking/NetworkController.hpp"
void Controller::keyEvent(GLFWwindow *w, mgl::Key key, int scancode, mgl::KeyAction action, mgl::Mods mods) {
	if (GameStateController::mode() == ProgramMode::Client && m_keys) {
		if (GameStateController::state() == GameState::Normal) {
			if (action == mgl::KeyAction::press) {
				if (key == *m_keys->move_up)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::move_up, true);
				else if (key == *m_keys->move_down)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::move_down, true);
				else if (key == *m_keys->move_left)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::move_left, true);
				else if (key == *m_keys->move_right)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::move_right, true);
				else if (key == *m_keys->anti_gravity)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::anti_gravity);
				else if (key == *m_keys->pause)
					GameStateController::pause();				
			} else if (action == mgl::KeyAction::release) {
				if (key == *m_keys->move_up)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::move_up, false);
				else if (key == *m_keys->move_down)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::move_down, false);
				else if (key == *m_keys->move_left)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::move_left, false);
				else if (key == *m_keys->move_right)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::move_right, false);
			}
		} else if (GameStateController::state() == GameState::Pause) {
			//Pause controls.
			if (action == mgl::KeyAction::press && key == *m_keys->pause)
				GameStateController::pause();
		} else if (GameStateController::state() == GameState::Initialization) {
			//Loading controls.
		}
	}
}
void Controller::mouseButtonEvent(GLFWwindow *w, mgl::MouseButton button, mgl::MouseAction action, mgl::Mods mods) {
	if (GameStateController::mode() == ProgramMode::Client) {
		if (GameStateController::state() == GameState::Normal) {
			if (action == mgl::MouseAction::press) {
				if (button == mgl::MouseButton::left)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::left_weapon, true);
				else if (button == mgl::MouseButton::right)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::right_weapon, true);
			} else if (action == mgl::MouseAction::release) {
				if (button == mgl::MouseButton::left)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::left_weapon, false);
				else if (button == mgl::MouseButton::right)
					NetworkController::send_control_event(m_actor, NetworkController::ControlEvent::right_weapon, false);
			}
		} else if (GameStateController::state() == GameState::Pause) {
			//Pause controls.
		} else if (GameStateController::state() == GameState::Initialization) {
			//Loading controls.
		}
	}
}

#include "../MyGraphicsLibrary/MGL/OpenGL/FunctionsMirror/ExperimentalFunctionsMirror.hpp"
void Controller::mouseMoveEvent(GLFWwindow *w, double x, double y) {
	if (GameStateController::mode() == ProgramMode::Client) {
		if (GameStateController::state() == GameState::Normal) {
			if (!current_screen_width || !current_screen_height)
				mgl::getWindowSize(w, &current_screen_width, &current_screen_height);
			auto temp_x = m_controlledCamera->minX();
			auto temp_y = m_controlledCamera->minY();
			float tx = float(x) / current_screen_width * (m_controlledCamera->maxX() - temp_x) + temp_x;
			float ty = float(y) / current_screen_height * (m_controlledCamera->maxY() - temp_y) + temp_y;
			m_actor->aim(tx, ty);
		}
	}
}
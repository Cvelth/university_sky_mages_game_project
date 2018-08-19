#include "Window.hpp"
#include "Client/Controller/Camera.hpp"
#include "Client/Controller/ControllerInterface.hpp"
#include "Engines/Graphics/MyGraphicsLibraryEngine.hpp"

Window::Window(const char* title, size_t width, size_t height, bool isFullscreen, 
			   MainActorQueue &main_actor_queue, DoubleProjectileQueue &projectile_queue, ObjectQueue &object_queue)
		: m_graphics(new MyGraphicsLibraryEngine(title, width, height, isFullscreen, main_actor_queue, projectile_queue, object_queue)), m_controller(nullptr), m_camera(nullptr), m_update_interval(16666) {}

void Window::changeController(ControllerInterface *controller, bool deleteOldOne) {
	if (m_controller) m_controller->stopCameraControl();
	m_graphics->insertController(controller);
	if (deleteOldOne && m_controller)
		delete m_controller;
	m_controller = controller;
	if (m_camera) m_controller->startCameraControl(m_camera);
}
void Window::insertCamera(std::shared_ptr<Camera> camera) {
	m_camera = camera;
	if (m_controller) m_controller->startCameraControl(m_camera);
}
float Window::currentAspectRatio() const {
	return float(m_graphics->width()) / m_graphics->height();
}
bool Window::isWindowClosed() {
	return m_graphics->isWindowClosed();
}
void Window::insertHUDRenderInfo(HUD_RenderInfo *hud) {
	m_graphics->insertHUD(hud);
}
Window::~Window() {
	delete m_graphics;
}

#include <chrono>
#include <thread>
#include <iostream>
#include "Shared/GameStateController.hpp"
void Window::loop(bool destroy_window_after_exit) {
	m_graphics->initializeMapRendering(m_camera);
	m_graphics->initializeQueueRendering();
	m_graphics->initializeHUDRendering();
	GameStateController::change_graphics_loop_state(true);

	while (!m_graphics->isWindowClosed()) {
		if (GameStateController::state() == GameState::Normal) {
			auto next_tick = std::chrono::steady_clock::now() + std::chrono::microseconds(getUpdateInterval());
				m_graphics->clearWindow();

				m_graphics->recalculateCamera();
				m_graphics->renderMap();
				m_graphics->renderQueues();
				m_graphics->renderHUD();
				m_graphics->update();

				std::this_thread::sleep_until(next_tick);
				m_graphics->pollEvents();
		} else if (GameStateController::state() == GameState::Pause) {
			m_graphics->clearWindow();

			//Pause menu.

			m_graphics->update();
			m_graphics->waitEvents();
		} else {

			//Loading menu.

			m_graphics->waitEvents();
		}
	}

	GameStateController::change_graphics_loop_state(false);
	m_graphics->cleanMapRendering();
	m_graphics->cleanQueueRendering();
	m_graphics->cleanHUDRendering();
	if (destroy_window_after_exit)
		delete this;
} 
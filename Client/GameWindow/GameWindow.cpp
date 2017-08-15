#include "GameWindow.hpp"
#include "Engines\Camera\Camera.hpp"
#include "Client\Controller\ControllerInterface.hpp"
#include "Engines\Graphics\MyGraphicsLibraryEngine.hpp"

void GameWindow::initialize() {
	//Does nothing.
}

void GameWindow::clean() {
	//Does nothing.
}

GameWindow::GameWindow(const char* title, size_t width, size_t height, bool isFullscreen, ObjectQueue *object_queue)
		: m_controller(nullptr), m_camera(nullptr), m_update_interval(16666) {

	m_graphics = new MyGraphicsLibraryEngine;

	initialize();
	m_graphics->initialize();
	m_graphics->initializeWindow(title, width, height, isFullscreen);
	m_graphics->initializeQueue(object_queue);
}

void GameWindow::insertController(ControllerInterface *controller) {
	m_graphics->insertController(controller);
	m_controller = controller;
	if (m_camera) m_controller->startCameraControl(m_camera);
}

void GameWindow::changeController(ControllerInterface *controller, bool deleteOldOne) {
	m_controller->stopCameraControl();
	m_graphics->insertController(controller);
	if (deleteOldOne && m_controller)
		delete m_controller;
	m_controller = controller;
	if (m_camera) m_controller->startCameraControl(m_camera);
}

void GameWindow::insertCamera(Camera *camera) {
	m_camera = camera;
	if (m_controller) m_controller->startCameraControl(m_camera);
}

void GameWindow::changeUpdateInterval(size_t microseconds) {
	m_update_interval = microseconds;
}

size_t GameWindow::getUpdateInterval() {
	return m_update_interval;
}

float GameWindow::currentAspectRatio() const {
	return float(m_graphics->width()) / m_graphics->height();
}

bool GameWindow::isWindowClosed() {
	return m_graphics->isWindowClosed();
}

void GameWindow::addToRenderQueue(IndependentObject *go) {
	m_graphics->addToRenderQueue(go);
}

void GameWindow::insertHUDRenderInfo(HUD_RenderInfo *hud) {
	m_graphics->insertHUD(hud);
}

GameWindow::~GameWindow() {
	m_graphics->clean();
	clean();
}

#include <chrono>
#include <thread>
#include <iostream>
#include "Shared\GameMode.hpp"
void GameWindow::loop(bool destroy_window_after_exit) {
	m_graphics->initializeMapRendering(m_camera);
	m_graphics->initializeQueueRendering();
	m_graphics->initializeHUDRendering();
	GameModeController::graphicsLoopIsReady(true);

	while (!m_graphics->isWindowClosed()) {
		if (GameModeController::getCurrentGameMode() == GameMode::Normal) {
			auto next_tick = std::chrono::steady_clock::now() + std::chrono::microseconds(getUpdateInterval());
			m_graphics->clearWindow();
			
			m_graphics->renderMap();
			m_graphics->renderQueue();
			m_graphics->renderHUD();
			m_graphics->update();
			
			std::this_thread::sleep_until(next_tick);
			m_graphics->pollEvents();
		} else if (GameModeController::getCurrentGameMode() == GameMode::Pause) {
			m_graphics->clearWindow();
			m_graphics->update();
			m_graphics->waitEvents();
		} else {
			m_graphics->waitEvents();
		}
	}

	GameModeController::graphicsLoopIsReady(false);
	m_graphics->cleanMapRendering();
	m_graphics->cleanQueueRendering();
	m_graphics->cleanHUDRendering();
	if (destroy_window_after_exit)
		delete this;
} 
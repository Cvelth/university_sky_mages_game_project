#include "GameWindow.hpp"
#include "LogicEngine\GameCamera.hpp"
#include "Controller\GameControllerInterface.hpp"
#include "GraphicsEngine\MyGraphicsLibraryEngine.hpp"

void GameWindow::initialize() {
	//Does nothing.
}

void GameWindow::clean() {
	//Does nothing.
}

GameWindow::GameWindow(const char* title, size_t width, size_t height, bool isFullscreen)
		: isMapInserted(false), isControllerInserted(false), m_update_interval(16666) {

	m_graphics = new MyGraphicsLibraryEngine;

	initialize();
	m_graphics->initialize();
	m_graphics->initializeWindow(title, width, height, isFullscreen);
	m_graphics->initializeQueue();
}

void GameWindow::insertController(GameControllerInterface* controller) {
	m_graphics->insertController(controller);
	m_controller = controller;
	isControllerInserted = true;
}

void GameWindow::changeController(GameControllerInterface* controller, bool deleteOldOne) {
	m_graphics->insertController(controller);
	if (deleteOldOne && isControllerInserted && m_controller)
		delete m_controller;
	m_controller = controller;
	isControllerInserted = true;
}

void GameWindow::insertMap(GameMap* map) {
	m_camera = new GameCamera(map, float(m_graphics->width()) / m_graphics->height());
	if (isControllerInserted) m_controller->startCameraControl(m_camera);
	isMapInserted = true;
}

void GameWindow::changeUpdateInterval(size_t microseconds) {
	m_update_interval = microseconds;
}

size_t GameWindow::getUpdateInterval() {
	return m_update_interval;
}

bool GameWindow::isWindowClosed() {
	return m_graphics->isWindowClosed();
}

void GameWindow::addToRenderQueue(AbstractGameObject *go) {
	m_graphics->addToRenderQueue(go);
}

GameWindow::~GameWindow() {
	if (isMapInserted && m_camera) {
		m_controller->stopCameraControl();
		delete m_camera;
	}

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
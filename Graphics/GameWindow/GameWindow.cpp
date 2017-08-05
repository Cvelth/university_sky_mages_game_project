#include "GameWindow.hpp"
#include "GameLogicEngine\GameCamera.hpp"
#include "Controller\GameControllerInterface.hpp"
#include "Exceptions\WindowExceptions.hpp"

#ifdef OPENGL_ENGINE_USED
#include "Graphics\OpenGLGraphicsEngine\OpenGLGraphicsEngine.hpp"
#endif
#ifdef VULKAN_ENGINE_USED
#include "Graphics\VulkanGraphicsEngine\VulkanGraphicsEngine.hpp"
#endif

void GameWindow::initialize() {
	//Does nothing.
}

void GameWindow::clean() {
	//Does nothing.
}

GameWindow::GameWindow(const char* title, size_t width, size_t height, bool isFullscreen)
		: isMapInserted(false), isControllerInserted(false), m_update_interval(16666) {
	
#ifdef OPENGL_ENGINE_USED
	m_graphics = new OpenGLGraphicsEngine();
#endif
#ifdef VULKAN_ENGINE_USED
	m_graphics = new VulkanGraphicsEngine();
#endif

	initialize();
	m_graphics->initialize();
	m_graphics->createWindow(title, width, height, isFullscreen);
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

RenderQueue* GameWindow::getRenderQueue() {
	return m_graphics->getRenderQueue();
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
void GameWindow::loop(bool destroy_window_after_exit) {
	m_graphics->initializeMapRendering(m_camera);
	m_graphics->initializeQueueRendering();

	while (!m_graphics->isWindowClosed()) {
		auto begin_time = std::chrono::steady_clock::now();
		auto next_tick = begin_time + std::chrono::microseconds(getUpdateInterval());

		m_graphics->renderMap();
		m_graphics->renderQueue();
		m_graphics->update();

		auto end_time = std::chrono::steady_clock::now();
		std::cout << "Rendering of a frame took " << float((end_time - begin_time).count()) / 1.e+9f << " of a second.\n";
		//if (end_time > next_tick)
		//	throw Exceptions::RenderingIsTooSlowException(float((end_time - begin_time).count()) / 1.e+9f);
		std::this_thread::sleep_until(next_tick);

		m_graphics->pollEvents();
	}

	m_graphics->cleanMapRendering();
	m_graphics->cleanQueueRendering();
	if (destroy_window_after_exit)
		delete this;
} 
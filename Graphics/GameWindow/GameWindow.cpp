#include "GameWindow.hpp"
#include "GameLogicEngine\GameCamera.hpp"
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

GameWindow::GameWindow(char* title, size_t width, size_t height, bool isFullscreen)
		: isMapInserted(false), m_update_interval(16666) {
	
#ifdef OPENGL_ENGINE_USED
	m_graphics = new OpenGLGraphicsEngine();
#endif
#ifdef VULKAN_ENGINE_USED
	m_graphics = new VulkanGraphicsEngine();
#endif

	initialize();
	m_graphics->createWindow(title, width, height, isFullscreen);
	m_graphics->initialize();
}

void GameWindow::insertController(GameControllerInterface* controller) {
	m_graphics->insertController(controller);
}

void GameWindow::insertMap(GameMap* map) {
	m_camera = new GameCamera(map, float(m_graphics->width()) / m_graphics->height());
	isMapInserted = true;
}

void GameWindow::changeUpdateInterval(size_t microseconds) {
	m_update_interval = microseconds;
}

size_t GameWindow::getUpdateInterval() {
	return m_update_interval;
}

GameWindow::~GameWindow() {
	if (isMapInserted && m_camera) delete m_camera;

	m_graphics->clean();
	clean();
}

#include <chrono>
#include <thread>
#include <iostream>
void GameWindow::loop() {
	m_graphics->initializeMapRendering(m_camera);
	//m_graphics->initializeRenderProcess();

	while (!m_graphics->isWindowClosed()) {
		auto begin_time = std::chrono::steady_clock::now();
		auto next_tick = begin_time + std::chrono::microseconds(getUpdateInterval());
		m_graphics->renderMap();
		//m_graphics->renderProcess();
		m_graphics->update();
		auto end_time = std::chrono::steady_clock::now();
		std::cout << "Rendering of a frame took " << float((end_time - begin_time).count()) / 1.e+9f << " of a second.\n";
		//if (end_time > next_tick)
		//	throw Exceptions::RenderingIsTooSlowException(float((end_time - begin_time).count()) / 1.e+9f);
		std::this_thread::sleep_until(next_tick);
		m_graphics->pollEvents();
	}

	m_graphics->cleanMapRendering();
	//m_graphics->clearRenderProcess();
} 
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

GameWindow::GameWindow(char* title, size_t width, size_t height, bool isFullscreen, GameMap* map)
		: m_camera(new GameCamera(map, float(width) / height)) {
	
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

GameWindow::~GameWindow() {
	if (m_camera) delete m_camera;

	m_graphics->clean();
	m_graphics->destroyWindow();
	clean();
}

int GameWindow::loop() {
	m_graphics->initializeRenderProcess();
	while (!m_graphics->isWindowClosed())
		m_graphics->renderProcess();
	m_graphics->clearRenderProcess();
	return 0;
}
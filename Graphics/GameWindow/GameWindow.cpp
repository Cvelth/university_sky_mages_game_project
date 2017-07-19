#include "GameWindow.hpp"
#include "GameLogicEngine\GameCamera.hpp"
#include "Exceptions\WindowExceptions.hpp"

#ifdef OPENGL_ENGINE_USED
#include "Graphics\OpenGLGraphicsEngine\OpenGLGraphicsEngine.hpp"
#endif
#ifdef VULKAN_ENGINE_USED
#include "Graphics\VulkanGraphicsEngine\VulkanGraphicsEngine.hpp"
#endif

#include "glfw\glfw3.h"

void GameWindow::initialize() {
	if (!glfwInit())
		throw Exceptions::GLFWInitializationException();
}

void GameWindow::clean() {
	glfwTerminate();
}

GameWindow::GameWindow(char* title, size_t width, size_t height, GameMap* map, bool insertDefaultCallback) 
		: m_width(width), m_height(height), m_camera(new GameCamera(map, float(width) / height)) {
	initializeEngine();
	initialize();
	m_window = m_graphics->createWindow(title, width, height);
	m_graphics->initialize();

	if (insertDefaultCallback)
		glfwSetErrorCallback(GameWindow::errorCallback);
}

GameWindow::~GameWindow() {
	if (m_camera) delete m_camera;

	m_graphics->clean();
	m_graphics->destroyWindow();
	clean();
}

int GameWindow::loop() {
	m_graphics->initializeRenderProcess();
	while (!glfwWindowShouldClose(m_window))
		m_graphics->renderProcess();
	m_graphics->clearRenderProcess();
	return 0;
}

void GameWindow::errorCallback(int errorCode, const char* description) {
	throw Exceptions::GLFWCallBackException(errorCode, description);
}

#include "GameWindow.hpp"
#include "GameCamera.hpp"
#include "WindowExceptions.hpp"
#include "VulkanGraphicsEngine.hpp"

#include "glfw3.h"

void GameWindow::initialize() {
	if (!glfwInit())
		throw Exceptions::GLFWInitializationException();
}

void GameWindow::clean() {
	glfwTerminate();
}

GameWindow::GameWindow(char* title, size_t width, size_t height, GameMap* map, bool insertDefaultCallback) 
		: width(width), height(height), camera(new GameCamera(map, float(width) / height)) {
	graphics = new VulkanGraphicsEngine();
	initialize();
	window = graphics->createWindow(title, width, height);
	graphics->initialize();

	if (insertDefaultCallback)
		glfwSetErrorCallback(GameWindow::errorCallback);
}

GameWindow::~GameWindow() {
	if (camera) delete camera;

	graphics->clean();
	graphics->destroyWindow();
	clean();
}

int GameWindow::loop() {
	graphics->initializeRenderProcess();
	while (!glfwWindowShouldClose(window))
		graphics->renderProcess();
	graphics->clearRenderProcess();
	return 0;
}

void GameWindow::errorCallback(int errorCode, const char* description) {
	throw Exceptions::GLFWCallBackException(errorCode, description);
}

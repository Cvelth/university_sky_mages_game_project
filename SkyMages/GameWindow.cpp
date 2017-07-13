#include "GameWindow.hpp"
#include "GameCamera.hpp"
#include "WindowExceptions.hpp"

#include "glfw3.h"

void GameWindow::initializeGLFW() {
	if (!glfwInit())
		throw Exceptions::GLFWInitializationException();
}

void GameWindow::cleanGLFW() {
	glfwTerminate();
}

void GameWindow::cleanWindow() {
	glfwDestroyWindow(window);
}

GameWindow::GameWindow(char* title, size_t width, size_t height, GameMap* map, bool insertDefaultCallback) 
		: width(width), height(height), camera(new GameCamera(map, float(width) / height)) {
	initializeGLFW();
	initializeWindow(title);
	initializeGraphics();

	if (insertDefaultCallback)
		glfwSetErrorCallback(GameWindow::errorCallback);
}

GameWindow::~GameWindow() {
	if (camera) delete camera;

	cleanGraphics();
	cleanWindow();
	cleanGLFW();
}

int GameWindow::loop() {
	initializeRenderProcess();
	while (!glfwWindowShouldClose(window))
		renderProcess();
	clearRenderProcess();
	return 0;
}

void GameWindow::errorCallback(int errorCode, const char* description) {
	throw Exceptions::GLFWCallBackException(errorCode, description);
}

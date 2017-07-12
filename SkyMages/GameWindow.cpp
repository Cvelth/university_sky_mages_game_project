#include "GameWindow.hpp"
#include "glfw3.h"

void GameWindow::initializeGLFW() {
	if (!glfwInit())
		throw GLFWInitializationException();
}

void GameWindow::initializeWindow() {
	window = glfwCreateWindow(1280, 1024, "SkyMages Dev", 0, 0);
	if (!window)
		throw WindowInitializationException();
}

GameWindow::GameWindow(char* name, size_t width, size_t height, bool insertDefaultCallback) {
	initializeGLFW();

	initializeWindow();
	makeCurrent();

	if (insertDefaultCallback)
		glfwSetErrorCallback(GameWindow::errorCallback);
}

GameWindow::~GameWindow() {
	glfwTerminate();
}

int GameWindow::loop() {
	initializeGL();
	while (!glfwWindowShouldClose(window)) {
		updateGL();
	}
	clearGL();
	return 0;
}

void GameWindow::makeCurrent() {
	glfwMakeContextCurrent(window);
}

void GameWindow::errorCallback(int errorCode, const char* description) {
	throw GLFWCallBackException();
}

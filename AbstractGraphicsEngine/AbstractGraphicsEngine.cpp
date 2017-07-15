#include "AbstractGraphicsEngine.hpp"
#include "Exceptions\WindowExceptions.hpp"

#include "glfw\glfw3.h"

AbstractGraphicsEngine::AbstractGraphicsEngine() {}

AbstractGraphicsEngine::~AbstractGraphicsEngine() {}

GLFWwindow * AbstractGraphicsEngine::createWindow(char* title, size_t width, size_t height) {
	setWindowSettings();

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
		throw Exceptions::WindowInitializationException();
	return window;
}

void AbstractGraphicsEngine::destroyWindow() {
	glfwDestroyWindow(window);
}
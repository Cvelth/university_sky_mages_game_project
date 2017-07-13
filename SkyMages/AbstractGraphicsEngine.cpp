#include "AbstractGraphicsEngine.hpp"
#include "WindowExceptions.hpp"

#include "vulkan.h"
#include "glfw3.h"

AbstractGraphicsEngine::AbstractGraphicsEngine() {}

AbstractGraphicsEngine::~AbstractGraphicsEngine() {}

GLFWwindow * AbstractGraphicsEngine::createWindow(char* title, size_t width, size_t height) {
	setWindowSettings();

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
		throw Exceptions::WindowInitializationException();
	return window;
}

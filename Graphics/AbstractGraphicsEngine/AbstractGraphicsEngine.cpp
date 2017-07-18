#include "AbstractGraphicsEngine.hpp"
#include "Exceptions\WindowExceptions.hpp"

#include "glfw\glfw3.h"

AbstractGraphicsEngine::AbstractGraphicsEngine() {}

AbstractGraphicsEngine::~AbstractGraphicsEngine() {}

GLFWwindow * AbstractGraphicsEngine::createWindow(char* title, size_t width, size_t height) {
	setWindowSettings();

	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!m_window)
		throw Exceptions::WindowInitializationException();
	m_window_width = width;
	m_window_height = height;
	return m_window;
}

void AbstractGraphicsEngine::destroyWindow() {
	glfwDestroyWindow(m_window);
}
#include "AbstractGraphicsEngine.hpp"
#include "Exceptions\WindowExceptions.hpp"

//#include "glfw\glfw3.h"

AbstractGraphicsEngine::AbstractGraphicsEngine() {}

AbstractGraphicsEngine::~AbstractGraphicsEngine() {}

void AbstractGraphicsEngine::createWindow(char* title, size_t width, size_t height, bool isFullscreen) {
	initializeWindow(title, width, height, isFullscreen); //glfwCreateWindow((int) width, (int) height, title, nullptr, nullptr);
	if (!m_window)
		throw Exceptions::WindowInitializationException();
	m_window_width = width;
	m_window_height = height;
}
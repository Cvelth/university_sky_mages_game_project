#include "AbstractGraphicsEngine.hpp"
#include "Shared\AbstractException.hpp"
DefineNewException(GraphicEngineInitializationException);

AbstractGraphicsEngine::AbstractGraphicsEngine() : m_queue(nullptr) {}

void AbstractGraphicsEngine::createWindow(const char* title, size_t width, size_t height, bool isFullscreen) {
	initializeWindow(title, width, height, isFullscreen);
	if (!m_window)
		throw Exceptions::GraphicEngineInitializationException("Window cannot be created.");
	m_window_width = width;
	m_window_height = height;
}

#include "RenderQueue.hpp"
void AbstractGraphicsEngine::initializeQueue() {
	m_queue = new RenderQueue;
}

AbstractGraphicsEngine::~AbstractGraphicsEngine() {
	if (m_queue) delete m_queue;
}
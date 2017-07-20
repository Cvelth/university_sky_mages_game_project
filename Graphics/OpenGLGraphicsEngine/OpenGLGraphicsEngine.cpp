#include "OpenGLGraphicsEngine.hpp"
#include "InnerOpenGLGraphicsEngine.hpp"

void OpenGLGraphicsEngine::initialize() {}

void OpenGLGraphicsEngine::clean() {}

void OpenGLGraphicsEngine::initializeWindow(char * title, size_t width, size_t height, bool isFullscreen) {
	m_engine->initialize(title, (int) width, (int) height,
							   isFullscreen ? mgl::DefaultWindowMode::Fullscreen : mgl::DefaultWindowMode::Windowed);
}

void OpenGLGraphicsEngine::destroyWindow() {
	m_engine->clean();
}

bool OpenGLGraphicsEngine::isWindowClosed() {
	return m_engine->isWindowClosed();
}

void OpenGLGraphicsEngine::initializeRenderProcess() {
	m_engine->initializeRenderProcess();
}

void OpenGLGraphicsEngine::renderProcess() {
	m_engine->renderProcess();
}

void OpenGLGraphicsEngine::clearRenderProcess() {
	m_engine->clearRenderProcess();
}

OpenGLGraphicsEngine::OpenGLGraphicsEngine() {
	m_engine = new InnerOpenGLGraphicsEngine();
}

OpenGLGraphicsEngine::~OpenGLGraphicsEngine() {
	delete m_engine;
}

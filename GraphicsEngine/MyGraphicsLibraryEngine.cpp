#include "MyGraphicsLibraryEngine.hpp"
#include "MGL_Window.hpp"

MGL_FORCE_NVIDIA_GPU_USAGE

void MyGraphicsLibraryEngine::initialize() {

}

void MyGraphicsLibraryEngine::clean() {
	delete m_window;
}

void MyGraphicsLibraryEngine::initializeWindow(const char * title, size_t width, size_t height, bool isFullscreen) {
	m_window->initialize(title, (int) width, (int) height,
						 isFullscreen ? mgl::DefaultWindowMode::Fullscreen : mgl::DefaultWindowMode::Windowed);
}

void MyGraphicsLibraryEngine::destroyWindow() {
	m_window->clean();
}

bool MyGraphicsLibraryEngine::isWindowClosed() {
	return m_window->isWindowClosed();
}

size_t MyGraphicsLibraryEngine::width() const {
	return m_window->getWidth();
}

size_t MyGraphicsLibraryEngine::height() const {
	return m_window->getHeight();
}

void MyGraphicsLibraryEngine::update() {
	m_window->update();
}

void MyGraphicsLibraryEngine::pollEvents() {
	m_window->pollEvents();
}
MyGraphicsLibraryEngine::MyGraphicsLibraryEngine() {
	m_window = new MGLWindow();
}

#include "RenderTools\RenderQueue.hpp"
void MyGraphicsLibraryEngine::initializeQueue() {
	m_queue = new RenderQueue;
}
void MyGraphicsLibraryEngine::addToRenderQueue(AbstractGameObject * go) {
	m_queue->add(go);
}
MyGraphicsLibraryEngine::~MyGraphicsLibraryEngine() {
	destroyWindow();
	clean();
	if (m_queue) delete m_queue;
}

#include "Controller\GameControllerInterface.hpp"
#include "Controller\GameController.hpp"
void MyGraphicsLibraryEngine::insertController(GameControllerInterface* controller) {
	m_window->initializeEventHandling(controller->controller());
}

#include <string>
#include "Shared\AbstractException.hpp"
DefineNewException(GraphicsEngineInitializationException)
#include <fstream>
std::string readShader(std::string filename) {
	std::ifstream file;
	file.open("Shaders\\" + filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		file.open("..\\RenderTools\\Shaders\\" + filename, std::ios::ate | std::ios::binary);
		if (!file.is_open())
			throw Exceptions::GraphicsEngineInitializationException("Shader cannot be read.");
		return std::string(std::istreambuf_iterator<char>(std::ifstream("..\\RenderTools\\Shaders\\" + filename)), std::istreambuf_iterator<char>());
	}
	return std::string(std::istreambuf_iterator<char>(std::ifstream("Shaders\\" + filename)), std::istreambuf_iterator<char>());
}
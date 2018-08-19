#include "MyGraphicsLibraryEngine.hpp"
#include "MGL_Window.hpp"

MGL_FORCE_NVIDIA_GPU_USAGE

MyGraphicsLibraryEngine::MyGraphicsLibraryEngine(const char* title, size_t width, size_t height, bool isFullscreen, 
												 MainActorQueue &mq, DoubleProjectileQueue &pq, ObjectQueue &oq) 
												 : m_actor_queue(mq), m_projectile_queue(pq), m_miscellaneous_queue(oq) {
	m_window = new MGLWindow();
	m_window->initialize(title, (int) width, (int) height,
						 isFullscreen ? mgl::DefaultWindowMode::Fullscreen : mgl::DefaultWindowMode::Windowed);
}
MyGraphicsLibraryEngine::~MyGraphicsLibraryEngine() {
	m_window->clean();
	delete m_window;
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
void MyGraphicsLibraryEngine::clearWindow() {
	m_window->clearWindow();
}
void MyGraphicsLibraryEngine::update() {
	m_window->update();
}
void MyGraphicsLibraryEngine::pollEvents() {
	m_window->pollEvents();
}
void MyGraphicsLibraryEngine::waitEvents() {
	m_window->waitEvents();
}
void MyGraphicsLibraryEngine::insertHUD(HUD_RenderInfo *hud) {
	m_hud = hud;
}

#include "Client/Controller/ControllerInterface.hpp"
#include "Client/Controller/Controller.hpp"
void MyGraphicsLibraryEngine::insertController(ControllerInterface* controller) {
	m_window->initializeEventHandling(controller->controller());
}

#include <string>
#include "Shared/AbstractException.hpp"
DefineNewException(GraphicsEngineInitializationException)
#include <fstream>
std::string readShader(std::string filename) {
	std::ifstream file;
	auto shader_path_variants = { "Shaders\\", "..\\Engines\\Graphics\\Shaders\\", "..\\..\\Engines\\Graphics\\Shaders\\" };
	auto shader_path_iterator = shader_path_variants.begin();
	while (shader_path_iterator != shader_path_variants.end() && !file.is_open())
		file.open(*(shader_path_iterator++) + filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw Exceptions::GraphicsEngineInitializationException("Shader cannot be read.");

	return std::string(std::istreambuf_iterator<char>(std::ifstream(*(--shader_path_iterator) + filename)), std::istreambuf_iterator<char>());
}
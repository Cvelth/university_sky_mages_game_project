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
MyGraphicsLibraryEngine::MyGraphicsLibraryEngine() {
	m_window = new MGLWindow();
}

#include "Objects/ObjectState/ObjectQueue.hpp"
void MyGraphicsLibraryEngine::initializeQueues(MainActorQueue *mq, DoubleProjectileQueue *pq, ObjectQueue *oq) {
	if (mq)	m_actor_queue = mq;
	else m_actor_queue = new MainActorQueue();
	if (pq)	m_projectile_queue = pq;
	else m_projectile_queue = new DoubleProjectileQueue();
	if (oq)	m_miscellaneous_queue = oq;
	else m_miscellaneous_queue = new ObjectQueue();
}
//void MyGraphicsLibraryEngine::addToRenderQueue(IndependentObject* go) {
//	m_queue->add(go);
//}
void MyGraphicsLibraryEngine::insertHUD(HUD_RenderInfo *hud) {
	m_hud = hud;
}
MyGraphicsLibraryEngine::~MyGraphicsLibraryEngine() {
	destroyWindow();
	clean();
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
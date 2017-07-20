#include "InnerVulkanGraphicsEngine.hpp"
#include "Exceptions\VulkanExceptions.hpp"

VulkanGraphicsEngine::VulkanGraphicsEngine() : m_isInnerEngineInitialized(false) {}

VulkanGraphicsEngine::~VulkanGraphicsEngine() {
	if (m_isInnerEngineInitialized)
		delete m_engine;
}

void VulkanGraphicsEngine::initialize() {
	m_engine->initialize();
}

void VulkanGraphicsEngine::clean() {
	m_engine->clean();
}

void VulkanGraphicsEngine::initializeWindow(char * title, size_t width, size_t height, bool isFullscreen) {
	if (!glfwInit())
		throw Exceptions::GLFWInitializationException();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow((int) width, (int) height, title, nullptr, nullptr);
	m_engine = new InnerVulkanGraphicsEngine(m_window);
	m_isInnerEngineInitialized = true;
}

void VulkanGraphicsEngine::destroyWindow() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool VulkanGraphicsEngine::isWindowClosed() {
	return glfwWindowShouldClose(m_window);
}

void VulkanGraphicsEngine::initializeRenderProcess() {
	m_engine->initializeRenderProcess();
}

void VulkanGraphicsEngine::renderProcess() {
	m_engine->renderProcess();
}

void VulkanGraphicsEngine::clearRenderProcess() {
	m_engine->clearRenderProcess();
}

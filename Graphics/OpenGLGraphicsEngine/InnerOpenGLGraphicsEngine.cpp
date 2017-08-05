#include "InnerOpenGLGraphicsEngine.hpp"

InnerOpenGLGraphicsEngine::InnerOpenGLGraphicsEngine() {}

InnerOpenGLGraphicsEngine::~InnerOpenGLGraphicsEngine() {}

void InnerOpenGLGraphicsEngine::initializeRenderProcess() {
	setClearColor(1.f, 0.f, 0.f);
}
void InnerOpenGLGraphicsEngine::renderProcess() {
	clearWindow();
}
void InnerOpenGLGraphicsEngine::clearRenderProcess() {

}

#include "Shared\AbstractException.hpp"
DefineNewException(GraphicsEngineInitializationException)
#include <fstream>
std::string readShader(std::string filename) {
	std::ifstream file;
	file.open("Shaders\\" + filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		file.open("..\\Graphics\\Shaders\\Source\\" + filename, std::ios::ate | std::ios::binary);
		if (!file.is_open())
			throw Exceptions::GraphicsEngineInitializationException("Shader cannot be read.");
		return std::string(std::istreambuf_iterator<char>(std::ifstream("..\\Graphics\\Shaders\\Source\\" + filename)), std::istreambuf_iterator<char>());
	}
	return std::string(std::istreambuf_iterator<char>(std::ifstream("Shaders\\" + filename)), std::istreambuf_iterator<char>());
}
#include "InnerOpenGLGraphicsEngine.hpp"
#include "MyOpenGL.hpp"
//#include "OpenGL_Mirror\FunctionsMirror\FunctionsMirror.hpp"

void InnerOpenGLGraphicsEngine::initializeRenderProcess() {
	setClearColor(1.f, 0.f, 0.f);
}

void InnerOpenGLGraphicsEngine::renderProcess() {
	clearWindow();
	pollEvents();
	update();
}

void InnerOpenGLGraphicsEngine::clearRenderProcess() {

}
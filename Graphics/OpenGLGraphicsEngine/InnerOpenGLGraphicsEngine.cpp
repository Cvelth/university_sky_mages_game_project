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
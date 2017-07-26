#include "InnerOpenGLGraphicsEngine.hpp"
#include "MyOpenGL.hpp"
#include "OpenGL_Mirror\FunctionsMirror\FunctionsMirror.hpp"

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

void InnerOpenGLGraphicsEngine::resize(int width, int height) {
	m_aspectRatio = float(width) / height;
	mgl::setViewport(0, 0, width, height);

	if (m_projection) delete m_projection;
	m_projection = new mgl::math::Matrix(mgl::math::Matrix::orthographicMatrix(
		30.f * (m_aspectRatio > 1.f ? -m_aspectRatio : -1.f),
		30.f * (m_aspectRatio > 1.f ? m_aspectRatio : 1.f),
		30.f * (-1.f / (m_aspectRatio > 1.f ? 1.f : m_aspectRatio)),
		30.f * (+1.f / (m_aspectRatio > 1.f ? 1.f : m_aspectRatio)),
		30.f * (+1.f), 
		30.f * (-1.f)
	));
	//m_projection = new mgl::math::Matrix(mgl::math::Matrix::orthographicMatrix(-100, +100, -100, +100, -100, +100));
}

#include "OpenGL_Mirror\OpenGL_Dependency\OpenGL.h"
void InnerOpenGLGraphicsEngine::test() {
	glGenVertexArrays(1, &a);
	glBindVertexArray(a);
	glOrtho(-1, +1, +1, -1, -1, +1);
}

void InnerOpenGLGraphicsEngine::testD() {
	glBindVertexArray(a);
}

#include "InnerOpenGLGraphicsEngine.hpp"
#include "OpenGL_Mirror\FunctionsMirror\FunctionsMirror.hpp"

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
void InnerOpenGLGraphicsEngine::resize(int width, int height) {
	m_aspectRatio = float(width) / height;
	mgl::setViewport(0, 0, width, height);

	if (m_projection) delete m_projection;
	m_projection = new mgl::math::Matrix(mgl::math::Matrix::orthographicMatrix(
		1.f * (m_aspectRatio > 1.f ? -m_aspectRatio : -1.f),
		1.f * (m_aspectRatio > 1.f ? m_aspectRatio : 1.f),
		1.f * (-1.f / (m_aspectRatio > 1.f ? 1.f : m_aspectRatio)),
		1.f * (+1.f / (m_aspectRatio > 1.f ? 1.f : m_aspectRatio)),
		1.f * (+1.f),
		1.f * (-1.f)
	));
}
void InnerOpenGLGraphicsEngine::resize(GameCamera* camera) {
	auto size = getSize();
	resize(size.w, size.h, camera);
}
#include "GameLogicEngine\GameCamera.hpp"
void InnerOpenGLGraphicsEngine::resize(int width, int height, GameCamera* camera) {
	m_aspectRatio = float(width) / height;
	mgl::setViewport(0, 0, width, height);

	if (m_projection) delete m_projection;
	m_projection = new mgl::math::Matrix(mgl::math::Matrix::orthographicMatrix(
		camera->beginX() * (m_aspectRatio > 1.f ? -m_aspectRatio : -1.f),
		camera->endX()   * (m_aspectRatio > 1.f ? m_aspectRatio : 1.f),
		camera->endY()   * (-1.f / (m_aspectRatio > 1.f ? 1.f : m_aspectRatio)),
		camera->beginY() * (+1.f / (m_aspectRatio > 1.f ? 1.f : m_aspectRatio)),
		1.f * (+1.f),
		1.f * (-1.f)
	));
}
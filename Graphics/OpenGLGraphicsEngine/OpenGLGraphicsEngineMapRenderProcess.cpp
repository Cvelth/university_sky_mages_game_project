#include "OpenGLGraphicsEngine.hpp"
#include "InnerOpenGLGraphicsEngine.hpp"
#include "OpenGL_Mirror\FunctionsMirror\FunctionsMirror.hpp"
#include "Primitive\InstancingArray.hpp"

#include "GameLogicEngine\GameMap.hpp"
#include "GameLogicEngine\GameCamera.hpp"
#include "GameLogicEngine\AbstractBlock.hpp"
#include "Graphics\RenderInfo\RenderInfo.hpp"

void OpenGLGraphicsEngine::recalculateCamera() {
	if (m_camera->wasCameraChanged()) {
		recalculateProjection();
		recalculateInstancing();
		m_camera->cameraChangeWasHandled();
	}
}
void OpenGLGraphicsEngine::recalculateProjection() {
	m_engine->resize(m_camera);
	if (!m_map_program && m_map_program->isLinked())
		m_map_program->sendUniform(m_map_program.projection, *m_engine->projection());
	if (!m_queue_program && m_queue_program->isLinked())
		m_queue_program->sendUniform(m_map_program.projection, *m_engine->projection());
}
void OpenGLGraphicsEngine::recalculateInstancing() {
	auto minX = m_camera->minX_i();
	auto maxX = m_camera->maxX_i();
	auto minY = m_camera->minY_i();
	auto maxY = m_camera->maxY_i();
	m_map_program->use();
	for (auto p : m_map_program.translationInstances) {
		p.second->deleteAll();
		for (size_t x = minX; x <= maxX; x++)
			for (size_t y = minY; y <= maxY; y++)
				if (*m_camera->map()->get(x, y) == *p.first)
					p.second->insert(new mgl::math::Vector(float(x), float(y), 0.f, 0.f));
		p.second->send(mgl::DataUsage::StaticDraw);
		m_map_program.translation = m_map_program->enableAttrib("translation", 4, 0, 0);
		m_map_program->initializeInstancing(m_map_program.translation, 1);
	}
}

void InnerOpenGLGraphicsEngine::resize(GameCamera* camera) {
	auto size = getSize();
	resize(size.w, size.h, camera);
}
void InnerOpenGLGraphicsEngine::resize(int width, int height, GameCamera* camera) {
	m_aspectRatio = float(width) / height;
	mgl::setViewport(0, 0, width, height);
	auto t1 = camera->minX_f();
	auto t2 = camera->maxX_f();
	auto t3 = camera->maxY_f();
	auto t4 = camera->minY_f();
	if (m_projection) delete m_projection;
	m_projection = new mgl::math::Matrix(mgl::math::Matrix::orthographicMatrix(
		camera->minX_f() * (1.f / (m_aspectRatio > 1.f ? m_aspectRatio : 1.f)),
		camera->maxX_f() * (1.f / (m_aspectRatio > 1.f ? m_aspectRatio : 1.f)),
		camera->maxY_f() * (m_aspectRatio > 1.f ? 1.f : m_aspectRatio),
		camera->minY_f() * (m_aspectRatio > 1.f ? 1.f : m_aspectRatio),
		-1.f,
		+1.f
	));
}

void OpenGLGraphicsEngine::initializeMapRendering(GameCamera* camera) {
	m_map_program.program = m_engine->linkProgramWithDefaultFragmentShader(mgl::Shader::compileShaderSource(mgl::ShaderType::Vertex, readShader("MapVertexShader.glsl").c_str()));
	m_map_program->use();

	m_camera = camera;
	for (auto it : m_camera->map()->get_blocks_data()) {
		it->renderInfo()->get()->send(mgl::DataUsage::StaticDraw);
		m_map_program.translationInstances.push_back(std::pair<AbstractBlock*, mgl::InstancingArray*>(it, new mgl::InstancingArray()));
	}
	m_map_program->enableAttrib("position", 4, 8, 0);
	m_map_program->enableAttrib("color", 4, 8, 4);
	m_map_program.projection = m_map_program->getUniform("projection");

	m_engine->setClearColor(0.6f, 0.85f, 0.9f);
}

void OpenGLGraphicsEngine::renderMap() {
	recalculateCamera();

	m_engine->clearWindow();
	m_map_program->use();

	for (auto p : m_map_program.translationInstances) {
		p.first->renderInfo()->get()->draw(p.second);
	}
}

void OpenGLGraphicsEngine::cleanMapRendering() {
	for (auto it : m_camera->map()->get_blocks_data())
		it->renderInfo()->get()->clean();

	for (auto it : m_map_program.translationInstances)
		delete it.second;
	if (m_map_program.translation) delete m_map_program.translation;
	if (m_map_program.projection) delete m_map_program.projection;
	if (!m_map_program) delete *m_map_program;
}
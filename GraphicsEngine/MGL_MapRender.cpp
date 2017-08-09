#include "MyGraphicsLibraryEngine.hpp"
#include "MGL_Window.hpp"
#include "MGL\OpenGL\FunctionsMirror\FunctionsMirror.hpp"
#include "MGL\Primitive\InstancingArray.hpp"

#include "LogicEngine\GameMap.hpp"
#include "LogicEngine\GameCamera.hpp"
#include "LogicEngine\AbstractBlock.hpp"
#include "RenderTools\RenderInfo.hpp"

void MyGraphicsLibraryEngine::recalculateCamera() {
	m_camera->move();
	if (m_camera->wasCameraChanged()) {
		recalculateProjection();
		recalculateInstancing();
		m_camera->cameraChangeWasHandled();
	}
}
void MyGraphicsLibraryEngine::recalculateProjection() {
	m_window->resize(m_camera);
	if (!m_map_program && m_map_program->isLinked())
		m_map_program->sendUniform(m_map_program.projection, *m_window->projection());
	if (!m_queue_program && m_queue_program->isLinked())
		m_queue_program->sendUniform(m_map_program.projection, *m_window->projection());
}
void MyGraphicsLibraryEngine::recalculateInstancing() {
	auto minX = m_camera->minX_i();
	auto maxX = m_camera->maxX_i();
	auto minY = m_camera->minY_i();
	auto maxY = m_camera->maxY_i();
	if (minX != m_map_program.min_x || maxX != m_map_program.max_x ||
		minY != m_map_program.min_y || maxY != m_map_program.max_y) {
		m_map_program.min_x = minX;
		m_map_program.max_x = maxX;
		m_map_program.min_y = minY;
		m_map_program.max_y = maxY;

		m_map_program->use();
		for (auto p : m_map_program.translationInstances) {
			p.second->deleteAll();
			for (size_t x = minX; x <= maxX; x++)
				for (size_t y = minY; y <= maxY; y++)
					if (*m_camera->map()->get(x, y) == *p.first)
						p.second->insert(new mgl::math::vectorH(float(x), float(y), 0.f, 0.f));
			p.second->send(mgl::DataUsage::StaticDraw);
			m_map_program.translation = m_map_program->enableAttrib("translation", 4, 0, 0);
			m_map_program->initializeInstancing(m_map_program.translation, 1);
		}
	}
}

void MGLWindow::resize(GameCamera* camera) {
	auto size = getSize();
	resize(size.w, size.h, camera);
}
void MGLWindow::resize(int width, int height, GameCamera* camera) {
	m_aspectRatio = float(width) / height;
	mgl::setViewport(0, 0, width, height);
	if (m_projection) delete m_projection;
	m_projection = new mgl::math::Matrix(mgl::math::Matrix::orthographicMatrix(
		camera->minX() * (1.f / (m_aspectRatio > 1.f ? m_aspectRatio : 1.f)),
		camera->maxX() * (1.f / (m_aspectRatio > 1.f ? m_aspectRatio : 1.f)),
		camera->maxY() * (m_aspectRatio > 1.f ? 1.f : m_aspectRatio),
		camera->minY() * (m_aspectRatio > 1.f ? 1.f : m_aspectRatio),
		-1.f,
		+1.f
	));
}

void MyGraphicsLibraryEngine::initializeMapRendering(GameCamera* camera) {
	m_map_program.program = m_window->linkProgramWithDefaultFragmentShader(
		mgl::Shader::compileShaderSource(mgl::ShaderType::Vertex, 
										 readShader("MapVertexShader.glsl").c_str()));
	m_map_program->use();

	m_camera = camera;
	for (auto it : m_camera->map()->get_blocks_data()) {
		it->renderInfo()->get()->send(mgl::DataUsage::StaticDraw);
		m_map_program.translationInstances.push_back(std::pair<AbstractBlock*, mgl::InstancingArray*>(it, new mgl::InstancingArray()));
	}
	m_map_program->enableAttrib("position", 4, 8, 0);
	m_map_program->enableAttrib("color", 4, 8, 4);
	m_map_program.projection = m_map_program->getUniform("projection");

	m_window->setClearColor(0.6f, 0.85f, 0.9f);
}

void MyGraphicsLibraryEngine::renderMap() {
	recalculateCamera();
	m_map_program->use();

	for (auto p : m_map_program.translationInstances) {
		p.first->renderInfo()->get()->draw(p.second);
	}
}

void MyGraphicsLibraryEngine::cleanMapRendering() {
	for (auto it : m_camera->map()->get_blocks_data())
		it->renderInfo()->get()->clean();

	for (auto it : m_map_program.translationInstances)
		delete it.second;
	if (m_map_program.translation) delete m_map_program.translation;
	if (m_map_program.projection) delete m_map_program.projection;
	if (!m_map_program) delete *m_map_program;
}
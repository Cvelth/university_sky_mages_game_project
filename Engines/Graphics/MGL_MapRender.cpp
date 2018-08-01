#include "MyGraphicsLibraryEngine.hpp"
#include "MGL_Window.hpp"
#include "../MyGraphicsLibrary/MGL/OpenGL/FunctionsMirror/FunctionsMirror.hpp"
#include "../MyGraphicsLibrary/MGL/Primitive/InstancingArray.hpp"

#include "Objects/Map/Map.hpp"
#include "Objects/Map/Block.hpp"
#include "Engines/Camera/Camera.hpp"
#include "Engines/RenderTools/RenderInfo.hpp"

void MyGraphicsLibraryEngine::recalculateCamera() {
	m_camera->move();
	if (m_camera->wasCameraChanged()) {
		recalculateProjection();
		recalculateInstancing();
		m_camera->cameraChangeWasHandled();
	}
}
void resendSingleQueueProjection(QueueProgram &program_struct, mgl::math::Matrix const& projection) {
	if (!program_struct && program_struct->isLinked())
		program_struct->sendUniform(program_struct.projection, projection);
}
void MyGraphicsLibraryEngine::recalculateProjection() {
	m_window->resize(m_camera);
	if (!m_map_program && m_map_program->isLinked())
		m_map_program->sendUniform(m_map_program.projection, *m_window->projection());

	resendSingleQueueProjection(m_projectile_queue_program, *m_window->projection());
	resendSingleQueueProjection(m_miscellaneous_queue_program, *m_window->projection());
	resendSingleQueueProjection(m_actor_queue_program, *m_window->projection());
}
#include <iostream>
template <typename T> inline T const& diff(T const& v) { if (v < 0) return -v; else return v; }
void MyGraphicsLibraryEngine::recalculateInstancing() {
	auto minX = m_camera->minX_i();
	auto maxX = m_camera->maxX_i();
	auto minY = m_camera->minY_i();
	auto maxY = m_camera->maxY_i();

	if (minX != m_map_program.min_x || maxX != m_map_program.max_x ||
		minY != m_map_program.min_y || maxY != m_map_program.max_y) {

		std::cout << diff(minX - m_map_program.min_x) << '\t' << diff(maxX - m_map_program.max_x) << '\t'
				  << diff(minY - m_map_program.min_y) << '\t' << diff(maxY - m_map_program.max_y) << '\t';

		m_map_program->use();
		if (diff(minX - m_map_program.min_x) < 5 && diff(maxX - m_map_program.max_x) < 5 &&
			diff(minY - m_map_program.min_y) < 5 && diff(maxY - m_map_program.max_y)) {
		
			for (auto p : m_map_program.translationInstances) {
				if (minX < m_map_program.min_x)
					for (int x = m_map_program.min_x; x > minX; x++) {
						for ()
					}
				else

			}
		} else {
			for (auto p : m_map_program.translationInstances) {
				p.second->deleteAll();
				for (long long x = minX - 1; x <= maxX + 1; x++) {
					std::list<mgl::math::vectorH*> temp;
					for (long long y = minY - 1; y <= maxY + 1; y++)
						if (*m_camera->map()->get(x, y) == *p.first)
							temp.push_back(new mgl::math::vectorH(float(x), float(y), 0.f, 0.f));
					p.second->get().push_back(temp);
				}
			}
		}

		for (auto p : m_map_program.translationInstances) {
			p.second->send(mgl::DataUsage::DynamicDraw);
			m_map_program.translation = m_map_program->enableAttrib("translation", 4, 0, 0);
			m_map_program->initializeInstancing(m_map_program.translation, 1);
		}
		m_map_program.min_x = minX;
		m_map_program.max_x = maxX;
		m_map_program.min_y = minY;
		m_map_program.max_y = maxY;
	}
}

void MGLWindow::resize(Camera* camera) {
	auto size = getSize();
	resize(size.w, size.h, camera);
}
void MGLWindow::resize(int width, int height, Camera* camera) {
	m_aspectRatio = float(width) / height;
	mgl::setViewport(0, 0, width, height);
	if (m_projection) delete m_projection;

	auto minX = camera->minX();
	auto maxX = camera->maxX();
	auto minY = camera->minY();
	auto maxY = camera->maxY();
	m_projection = new mgl::math::Matrix(
		mgl::math::Matrix::orthographicMatrix(
			minX, maxX, maxY, minY, -1.f, +1.f));
}

void MyGraphicsLibraryEngine::initializeMapRendering(Camera* camera) {
	m_map_program.program = m_window->linkProgramWithDefaultFragmentShader(
		mgl::Shader::compileShaderSource(mgl::ShaderType::Vertex, 
										 readShader("MapVertexShader.glsl").c_str()));
	m_map_program->use();

	m_camera = camera;
	for (auto it : m_camera->map()->get_blocks_data()) {
		it->renderInfo()->get()->send(mgl::DataUsage::StaticDraw);
		m_map_program.translationInstances.push_back(std::make_pair(it, new mgl::InstancingMultiArray()));
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
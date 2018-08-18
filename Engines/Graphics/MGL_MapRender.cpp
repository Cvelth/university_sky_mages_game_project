#include "MyGraphicsLibraryEngine.hpp"
#include "MGL_Window.hpp"
#include "../MyGraphicsLibrary/MGL/OpenGL/FunctionsMirror/FunctionsMirror.hpp"
#include "../MyGraphicsLibrary/MGL/Primitive/InstancingArray.hpp"

#include "Objects/Map/Map.hpp"
#include "Objects/Map/Block.hpp"
#include "Client/Controller/Camera.hpp"
#include "Engines/Graphics/RenderInfo.hpp"
#include "Engines/ObjectStorage/RenderInfoStorage.hpp"

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
void MyGraphicsLibraryEngine::recalculateInstancing() {
	auto minX = m_camera->minX_i();
	auto maxX = m_camera->maxX_i();
	auto minY = m_camera->minY_i();
	auto maxY = m_camera->maxY_i();

	if (minX != m_map_program.min_x || maxX != m_map_program.max_x ||
		minY != m_map_program.min_y || maxY != m_map_program.max_y) {

		m_map_program->use();
		for (auto p : m_map_program.translationInstances) {
			p.second->initialize_data_edit();

			for (auto y = m_map_program.min_y; y < minY; y++)
				if (!p.second->get().empty())
					p.second->get().pop_front();
			for (auto y = m_map_program.max_y; y > maxY; y--)
				if (!p.second->get().empty())
					p.second->get().pop_back();

			for (auto y = m_map_program.min_y; y > minY; y--) {
				p.second->get().push_front(std::deque<std::unique_ptr<mgl::math::vectorH>>());
				for (auto x = m_map_program.min_x; x <= m_map_program.max_x; x++)
					if (*m_camera->map()->get(x, y) == *p.first)
						p.second->get().front().push_back(std::make_unique<mgl::math::vectorH>(float(x), float(y), 0.f, 0.f));
			}
			for (auto y = m_map_program.max_y; y < maxY; y++) {
				p.second->get().push_back(std::deque<std::unique_ptr<mgl::math::vectorH>>());
				for (auto x = m_map_program.min_x; x <= m_map_program.max_x; x++)
					if (*m_camera->map()->get(x, y) == *p.first)
						p.second->get().back().push_back(std::make_unique<mgl::math::vectorH>(float(x), float(y), 0.f, 0.f));
			}

			auto y = minY;
			for (auto &list : p.second->get()) {
				for (auto x = m_map_program.min_x; x < minX; x++)
					if (*m_camera->map()->get(x, y) == *p.first)
						if (!list.empty())
							list.pop_front();
				for (auto x = m_map_program.max_x; x > maxX; x--)
					if (*m_camera->map()->get(x, y) == *p.first)
						if (!list.empty())
							list.pop_back();

				for (auto x = m_map_program.min_x; x >= minX; x--)
					if (*m_camera->map()->get(x, y) == *p.first)
						list.push_front(std::make_unique<mgl::math::vectorH>(float(x), float(y), 0.f, 0.f));
				for (auto x = m_map_program.max_x; x <= maxX; x++)
					if (*m_camera->map()->get(x, y) == *p.first)
						list.push_back(std::make_unique<mgl::math::vectorH>(float(x), float(y), 0.f, 0.f));
				
				y++;
			}

			p.second->stop_data_edit();
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

void MGLWindow::resize(std::shared_ptr<Camera> camera) {
	auto size = getSize();
	resize(size.w, size.h, camera);
}
void MGLWindow::resize(int width, int height, std::shared_ptr<Camera> camera) {
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

void MyGraphicsLibraryEngine::initializeMapRendering(std::shared_ptr<Camera> camera) {
	m_map_program.program = m_window->linkProgramWithDefaultFragmentShader(
		mgl::Shader::compileShaderSource(mgl::ShaderType::Vertex, 
										 readShader("MapVertexShader.glsl").c_str()));
	m_map_program->use();

	m_camera = camera;
	for (auto it = m_camera->map()->get_blocks_data().rbegin(); it != m_camera->map()->get_blocks_data().rend(); it++) {
		(*it)->renderInfo()->get()->send(mgl::DataUsage::StaticDraw);
		m_map_program.translationInstances.push_back(std::make_pair(*it, new mgl::InstancingMultiArray()));
	}
	m_map_program->enableAttrib("position", 4, 8, 0);
	m_map_program->enableAttrib("color", 4, 8, 4);
	m_map_program.projection = m_map_program->getUniform("projection");
	
	m_window->setClearColor(*RenderInfoStorage::getPalette("Background").front());
}

void MyGraphicsLibraryEngine::renderMap() {
	m_map_program->use();

	for (auto p : m_map_program.translationInstances) {
		p.first->renderInfo()->get()->draw(p.second);
	}
}

void MyGraphicsLibraryEngine::cleanMapRendering() {
	if (m_camera) 
		for (auto it : m_camera->map()->get_blocks_data())
			it->renderInfo()->get()->clean();

	for (auto it : m_map_program.translationInstances)
		delete it.second;
	if (m_map_program.translation) delete m_map_program.translation;
	if (m_map_program.projection) delete m_map_program.projection;
	if (!m_map_program) delete *m_map_program;
}
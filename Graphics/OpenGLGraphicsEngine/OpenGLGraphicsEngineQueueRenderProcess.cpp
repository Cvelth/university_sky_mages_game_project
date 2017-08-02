#include "OpenGLGraphicsEngine.hpp"
#include "InnerOpenGLGraphicsEngine.hpp"
#include "OpenGL_Mirror\FunctionsMirror\FunctionsMirror.hpp"

#include "GameObjects\AbstractGameObject.hpp"
#include "Graphics\AbstractGraphicsEngine\RenderQueue.hpp"
#include "Graphics\RenderInfo\RenderInfo.hpp"
#include "GameLogicEngine\GameCamera.hpp"

void OpenGLGraphicsEngine::initializeQueueRendering() {
	m_queue_program.program = m_engine->linkProgramWithDefaultFragmentShader(mgl::Shader::compileShaderSource(mgl::ShaderType::Vertex, readShader("MapVertexShader.glsl").c_str()));
	m_queue_program->use();

	for (auto it : m_queue->get()) {
		if (it->isWaitingToBeInitilized()) {
			it->getRenderInto()->get()->send(mgl::DataUsage::DynamicDraw);
			it->initializeWasSuccessfull();
		}
		if (it->isWaitingToBeDestroyed()) {
			it->getRenderInto()->get()->clean();
			it->destructionWasSuccessfull();
		}
	}

	m_queue_program->enableAttrib("position", 4, 8, 0);
	m_queue_program->enableAttrib("color", 4, 8, 4);

	m_map_program.translation = m_queue_program->getUniform("translation");
	m_map_program.scaling = m_queue_program->getUniform("scaling");
	m_map_program.projection = m_queue_program->getUniform("projection");

	m_queue_program->sendUniform(m_map_program.translation, mgl::math::Vector(0.f, 0.f, 0.f, 0.f));
	m_queue_program->sendUniform(m_map_program.scaling, mgl::math::Vector(1.f, 1.f, 1.f, 1.f));
								 
	m_engine->resize(m_camera);	 
	m_queue_program->sendUniform(m_map_program.projection, *m_engine->projection());
}

void OpenGLGraphicsEngine::renderQueue() {
	if (m_camera->wasCameraChanged()) {
		m_engine->resize(m_camera);
		if (!m_map_program && m_map_program->isLinked())
			m_map_program->sendUniform(m_map_program.projection, *m_engine->projection());
		if (!m_queue_program && m_queue_program->isLinked())
			m_queue_program->sendUniform(m_map_program.projection, *m_engine->projection());
		m_camera->cameraChangeWasHandled();
	}

	m_queue_program->use();

	auto minX = m_camera->minX_i();
	auto maxX = m_camera->maxX_i();
	auto minY = m_camera->minY_i();
	auto maxY = m_camera->maxY_i();
	for (auto it : m_queue->get()) {
		auto position = it->position();
		if (position.h >= minX && position.v >= minY &&
			position.h <= maxX && position.v <= maxY) {

			m_queue_program->sendUniform(m_map_program.translation, mgl::math::Vector(position.h, position.v, 0.f, 0.f));
			it->getRenderInto()->get()->draw();
		}
	}
}

void OpenGLGraphicsEngine::cleanQueueRendering() {
	for (auto it : m_queue->get()) {
		it->getRenderInto()->get()->clean();
		it->destructionWasSuccessfull();
	}
}
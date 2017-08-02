#include "OpenGLGraphicsEngine.hpp"
#include "InnerOpenGLGraphicsEngine.hpp"
#include "OpenGL_Mirror\FunctionsMirror\FunctionsMirror.hpp"

#include "GameObjects\AbstractGameObject.hpp"
#include "Graphics\AbstractGraphicsEngine\RenderQueue.hpp"
#include "Graphics\RenderInfo\RenderInfo.hpp"
#include "GameLogicEngine\GameCamera.hpp"

void OpenGLGraphicsEngine::initializeQueueRendering() {
	m_queue_program.program = m_engine->linkProgramWithDefaultFragmentShader(mgl::Shader::compileShaderSource(mgl::ShaderType::Vertex, readShader("QueueVertexShader.glsl").c_str()));
	m_queue_program->use();

	m_queue->for_each([](AbstractGameObject* go) {
		if (go->isWaitingToBeInitilized()) {
			go->getRenderInto()->get()->send(mgl::DataUsage::DynamicDraw);
			go->initializeWasSuccessfull();
		}
		if (go->isWaitingToBeDestroyed()) {
			go->getRenderInto()->get()->clean();
			go->destructionWasSuccessfull();
		}
	});

	m_queue_program->enableAttrib("position", 4, 8, 0);
	m_queue_program->enableAttrib("color", 4, 8, 4);

	m_queue_program.translation = m_queue_program->getUniform("translation");
	m_queue_program.scaling = m_queue_program->getUniform("scaling");
	m_queue_program.projection = m_queue_program->getUniform("projection");

	m_queue_program->sendUniform(m_queue_program.translation, mgl::math::Vector(0.f, 0.f, 0.f, 0.f));
	m_queue_program->sendUniform(m_queue_program.scaling, mgl::math::Vector(1.f, 1.f, 1.f, 1.f));
}

void OpenGLGraphicsEngine::renderQueue() {
	recalculateCamera();

	m_queue_program->use();

	auto minX = m_camera->minX_i();
	auto maxX = m_camera->maxX_i();
	auto minY = m_camera->minY_i();
	auto maxY = m_camera->maxY_i();
	m_queue->for_each([&minX, &maxX, &minY, &maxY, this](AbstractGameObject* go) {
		auto position = go->position();
		if (position.h >= minX && position.v >= minY &&
			position.h <= maxX && position.v <= maxY) {

			m_queue_program->sendUniform(m_queue_program.translation, mgl::math::Vector(position.h, position.v, 0.f, 0.f));
			go->getRenderInto()->get()->draw();
		}
	});
}

void OpenGLGraphicsEngine::cleanQueueRendering() {
	m_queue->for_each([](AbstractGameObject* go) {
		go->getRenderInto()->get()->clean();
		go->destructionWasSuccessfull();
	});

	if (m_queue_program.translation) delete m_queue_program.translation;
	if (m_queue_program.scaling) delete m_queue_program.scaling;
	if (m_queue_program.projection) delete m_queue_program.projection;
	if (!m_queue_program) delete *m_queue_program;
}
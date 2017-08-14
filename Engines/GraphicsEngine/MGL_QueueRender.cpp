#include "MyGraphicsLibraryEngine.hpp"
#include "MGL_Window.hpp"
#include "MGL\OpenGL\FunctionsMirror\FunctionsMirror.hpp"

#include "Objects\GameObjects\AbstractGameObject.hpp"
#include "Objects\GameObjects\ObjectState.hpp"
#include "Objects\LogicEngine\GameCamera.hpp"
#include "Engines\RenderTools\RenderQueue.hpp"
#include "Engines\RenderTools\RenderInfo.hpp"

void MyGraphicsLibraryEngine::initializeQueueRendering() {
	m_queue_program.program = m_window->linkProgramWithDefaultFragmentShader(
		mgl::Shader::compileShaderSource(mgl::ShaderType::Vertex, 
										 readShader("QueueVertexShader.glsl").c_str()));
	m_queue_program->use();

	m_queue->for_each([this](AbstractGameObject* go) {
		if (go->isWaitingToBeInitilized()) {
			go->getRenderInto()->get()->send(mgl::DataUsage::StaticDraw);
			go->getRenderInto()->get()->insertVertexArray(m_queue_program->getVertexArray());
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

	m_queue_program->sendUniform(m_queue_program.translation, mgl::math::vectorH(0.f, 0.f, 0.f, 0.f));
	m_queue_program->sendUniform(m_queue_program.scaling, mgl::math::vectorH(1.f, 1.f, 1.f, 1.f));
}

void MyGraphicsLibraryEngine::renderQueue() {
	recalculateCamera();
	m_queue_program->use();

	auto minX = m_camera->minX_i();
	auto maxX = m_camera->maxX_i();
	auto minY = m_camera->minY_i();
	auto maxY = m_camera->maxY_i();
	m_queue->for_each([&minX, &maxX, &minY, &maxY, this](AbstractGameObject* go) {
		auto position = go->position();
		if (position[0] >= minX && position[1] >= minY &&
			position[0] <= maxX && position[1] <= maxY) {

			m_queue_program->sendUniform(m_queue_program.translation, mgl::math::vectorH(position[0], position[1], 0.f, 0.f));
			go->getRenderInto()->get()->draw();
		}
	});
}

void MyGraphicsLibraryEngine::cleanQueueRendering() {
	m_queue->for_each([](AbstractGameObject* go) {
		go->getRenderInto()->get()->clean();
		go->destructionWasSuccessfull();
	});

	if (m_queue_program.translation) delete m_queue_program.translation;
	if (m_queue_program.scaling) delete m_queue_program.scaling;
	if (m_queue_program.projection) delete m_queue_program.projection;
	if (!m_queue_program) delete *m_queue_program;
}
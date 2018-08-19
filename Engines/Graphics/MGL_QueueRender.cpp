#include "MyGraphicsLibraryEngine.hpp"
#include "MGL_Window.hpp"
#include "../MyGraphicsLibrary/MGL/OpenGL/FunctionsMirror/FunctionsMirror.hpp"
#include "Objects/ObjectState/ObjectQueue.hpp"
#include "Objects/AbstractObjects/IndependentObject.hpp"
#include "Objects/AbstractObjects/ShootableObject.hpp"
#include "Objects/Actors/MainActor.hpp"
#include "Client/Controller/Camera.hpp"
#include "Engines/Graphics/RenderInfo.hpp"

template <typename Type> 
inline void sendRenderInfo(std::shared_ptr<Type> go, mgl::VertexArray *va) {
	if (!go->getRenderInto()->isInitialized()) {
		go->getRenderInto()->insertVertexArray(va);
		go->getRenderInto()->send(mgl::DataUsage::StaticDraw);
	}
}

template <typename Type>
void initializeSingleQueueRendering(AbstractQueueInterface<Type> *queue, QueueProgram &queue_program, MGLWindow *window) {
	queue_program.program = window->linkProgramWithDefaultFragmentShader(
		mgl::Shader::compileShaderSource(mgl::ShaderType::Vertex,
			readShader("QueueVertexShader.glsl").c_str()));

	queue_program->use();

	queue->for_each([&queue_program](std::shared_ptr<Type> go) {
		sendRenderInfo(go, queue_program->getVertexArray());
	});

	queue_program->enableAttrib("position", 4, 8, 0);
	queue_program->enableAttrib("color", 4, 8, 4);

	queue_program.translation = queue_program->getUniform("translation");
	queue_program.scaling = queue_program->getUniform("scaling");
	queue_program.projection = queue_program->getUniform("projection");

	queue_program->sendUniform(queue_program.translation, mgl::math::vectorH(0.f, 0.f, 0.f, 0.f));
	queue_program->sendUniform(queue_program.scaling, mgl::math::vectorH(1.f, 1.f, 1.f, 1.f));
}

void MyGraphicsLibraryEngine::initializeQueueRendering() {
	initializeSingleQueueRendering(&m_projectile_queue->get(), m_projectile_queue_program, m_window);
	initializeSingleQueueRendering(m_miscellaneous_queue, m_miscellaneous_queue_program, m_window);
	initializeSingleQueueRendering(m_actor_queue, m_actor_queue_program, m_window);
}

template <typename Type, typename NumeralType>
void renderSingleQueue(AbstractQueueInterface<Type> *queue, QueueProgram &queue_program, NumeralType minX, NumeralType maxX, NumeralType minY, NumeralType maxY) {
	queue_program->use();
	queue->for_each([&minX, &maxX, &minY, &maxY, queue_program](std::shared_ptr<Type> go) {
		sendRenderInfo(go, queue_program->getVertexArray());
		auto position = go->position();
		if (position[0] >= minX && position[1] >= minY &&
			position[0] <= maxX && position[1] <= maxY) {

			queue_program->sendUniform(queue_program.translation, mgl::math::vectorH(position[0], position[1], 0.f, 0.f));
			go->getRenderInto()->draw();
		}
	});
}

void MyGraphicsLibraryEngine::renderQueues() {
	auto minX = m_camera->minX_i();
	auto maxX = m_camera->maxX_i();
	auto minY = m_camera->minY_i();
	auto maxY = m_camera->maxY_i();
	renderSingleQueue(&m_projectile_queue->get(), m_projectile_queue_program, minX, maxX, minY, maxY);
	renderSingleQueue(m_miscellaneous_queue, m_miscellaneous_queue_program, minX, maxX, minY, maxY);
	renderSingleQueue(m_actor_queue, m_actor_queue_program, minX, maxX, minY, maxY);
}

template <typename Type>
void cleanSingleQueueRendering(AbstractQueueInterface<Type> *queue, QueueProgram &queue_program) {
	queue->for_each([](std::shared_ptr<Type> go) {
		go->getRenderInto()->clean();
	});

	if (queue_program.translation) delete queue_program.translation;
	if (queue_program.scaling) delete queue_program.scaling;
	if (queue_program.projection) delete queue_program.projection;
	if (!queue_program) delete *queue_program;
}

void MyGraphicsLibraryEngine::cleanQueueRendering() {
	cleanSingleQueueRendering(&m_projectile_queue->get(), m_projectile_queue_program);
	cleanSingleQueueRendering(m_miscellaneous_queue, m_miscellaneous_queue_program);
	cleanSingleQueueRendering(m_actor_queue, m_actor_queue_program);
}
#include "OpenGLGraphicsEngine.hpp"
#include "InnerOpenGLGraphicsEngine.hpp"

void OpenGLGraphicsEngine::initialize() {
	m_program = m_engine->linkDefaultProgram(mgl::DefaulProgramType::VertexNoMatrices);
	m_program->use();
	m_program->enableAttrib("position", 8, false, 4, 0);
	m_program->enableAttrib("color", 8, false, 4, 4);
	m_program->sendUniform("translation", mgl::math::Vector(0.f, 0.f, 0.f, 0.f));
	m_program->sendUniform("scalation", mgl::math::Vector(1.f, 1.f, 1.f, 1.f));
}

void OpenGLGraphicsEngine::clean() {
	delete m_program;
	delete m_engine;
}

void OpenGLGraphicsEngine::initializeWindow(char * title, size_t width, size_t height, bool isFullscreen) {
	m_engine->initialize(title, (int) width, (int) height,
							   isFullscreen ? mgl::DefaultWindowMode::Fullscreen : mgl::DefaultWindowMode::Windowed);
	m_window = m_engine->window();
}

void OpenGLGraphicsEngine::destroyWindow() {
	m_engine->clean();
}

bool OpenGLGraphicsEngine::isWindowClosed() {
	return m_engine->isWindowClosed();
}

void OpenGLGraphicsEngine::update() {
	m_engine->update();
}

void OpenGLGraphicsEngine::pollEvents() {
	m_engine->pollEvents();
}

OpenGLGraphicsEngine::OpenGLGraphicsEngine() {
	m_engine = new InnerOpenGLGraphicsEngine();
}

OpenGLGraphicsEngine::~OpenGLGraphicsEngine() {
	clean();
}

#include "GameLogicEngine\GameMap.hpp"
#include "GameLogicEngine\GameCamera.hpp"
#include "GameLogicEngine\AbstractBlock.hpp"
#include "Graphics\RenderInfo\RenderInfo.hpp"
void OpenGLGraphicsEngine::initializeMapRendering(GameMap* map) {
	m_engine->setClearColor(0.6f, 0.85f, 0.9f);
	for (auto it : map->get_blocks_data())
		it->renderInfo()->get()->send(mgl::DataUsage::StaticDraw);
}

void OpenGLGraphicsEngine::renderMap(GameCamera* camera) {
	m_engine->clearWindow();
	for (int i = camera->beginX(); i < camera->endX(); i++)
		for (int j = camera->beginY(); j < camera->endY(); j++) {
			m_program->sendUniform("translation", mgl::math::Vector(i, j, -1.f, 0.f));
			camera->map()->get(i, j)->renderInfo()->get()->draw();
		}
}

void OpenGLGraphicsEngine::cleanMapRendering(GameMap* map) {
	for (auto it : map->get_blocks_data())
		it->renderInfo()->get()->clean();
}

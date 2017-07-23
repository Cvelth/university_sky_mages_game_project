#include "OpenGLGraphicsEngine.hpp"
#include "InnerOpenGLGraphicsEngine.hpp"

void OpenGLGraphicsEngine::initialize() {
	m_engine->linkDefaultProgram(mgl::DefaulProgramType::VertexNoMatrices);
}

void OpenGLGraphicsEngine::clean() {}

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

OpenGLGraphicsEngine::OpenGLGraphicsEngine() {
	m_engine = new InnerOpenGLGraphicsEngine();
}

OpenGLGraphicsEngine::~OpenGLGraphicsEngine() {
	delete m_engine;
}

#include "GameLogicEngine\GameMap.hpp"
#include "GameLogicEngine\GameCamera.hpp"
#include "GameLogicEngine\AbstractBlock.hpp"
#include "Graphics\RenderInfo\RenderInfo.hpp"
void OpenGLGraphicsEngine::initializeMapRendering(GameMap* map) {
	for (int i = 0; i < map->width(); i++)
		for (int j = 0; j < map->height(); j++)
			map->get(i, j)->renderInfo()->get()->send(mgl::DataUsage::StaticDraw);
}

void OpenGLGraphicsEngine::renderMap(GameCamera* camera) {
	for (int i = camera->beginX(); i < camera->endX(); i++)
		for (int j = camera->beginY(); j < camera->endY(); j++)
			camera->map()->get(i, j)->renderInfo()->get()->draw();
}

void OpenGLGraphicsEngine::cleanMapRendering(GameMap* map) {
	for (int i = 0; i < map->width(); i++)
		for (int j = 0; j < map->height(); j++)
			map->get(i, j)->renderInfo()->get()->clean();
}

#include "OpenGLGraphicsEngine.hpp"
#include "InnerOpenGLGraphicsEngine.hpp"
#include "Exceptions\OpenGLExceptions.hpp"

void OpenGLGraphicsEngine::initialize() {

}

void OpenGLGraphicsEngine::clean() {
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
std::string readShader(std::string name);
void OpenGLGraphicsEngine::initializeMapRendering(GameMap* map) {
	std::string source = readShader("MapVertexShader.glsl");
	auto index = source.find("@");
	if (index == -1)
		throw Exceptions::MapShaderCompilationException();
	source.erase(index, 1);
	source.insert(index, std::to_string(map->getSize()));
	auto map_shader = mgl::Shader::compileShaderSource(mgl::ShaderType::Vertex, source);
	m_map_program = m_engine->linkProgramWithDefaultFragmentShader(map_shader);

	//m_program = m_engine->linkDefaultProgram(mgl::DefaulProgramType::VertexNoMatrices);
	//m_program->use();
	//m_program->enableAttrib("position", 8, false, 4, 0);
	//m_program->enableAttrib("color", 8, false, 4, 4);
	//m_translation = m_program->getUniform("translation");
	//m_scaling = m_program->getUniform("scaling");
	//m_program->sendUniform(m_translation, mgl::math::Vector(0.f, 0.f, 0.f, 0.f));
	//m_program->sendUniform(m_scaling, mgl::math::Vector(1.f, 1.f, 1.f, 1.f));

	m_engine->setClearColor(0.6f, 0.85f, 0.9f);
	for (auto it : map->get_blocks_data())
		it->renderInfo()->get()->send(mgl::DataUsage::StaticDraw);
}

void OpenGLGraphicsEngine::renderMap(GameCamera* camera) {
	m_engine->clearWindow();
	for (int i = camera->beginX(); i < camera->endX(); i++)
		for (int j = camera->beginY(); j < camera->endY(); j++) {
			//m_program->sendUniform(m_translation, mgl::math::Vector(float(i), float(j), -1.f, 0.f));
			camera->map()->get(i, j)->renderInfo()->get()->draw();
		}
}

void OpenGLGraphicsEngine::cleanMapRendering(GameMap* map) {
	for (auto it : map->get_blocks_data())
		it->renderInfo()->get()->clean();

	if (m_map_offsets) delete m_map_offsets;
	if (m_map_program) delete m_map_program;
}

#include <fstream>
std::string readShader(std::string filename) {
	std::ifstream file;
	file.open("Shaders\\" + filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		file.open("..\\Graphics\\Shaders\\Source\\" + filename, std::ios::ate | std::ios::binary);
		if (!file.is_open())
			throw Exceptions::ShaderFileException();
		return std::string(std::istreambuf_iterator<char>(std::ifstream("..\\Graphics\\Shaders\\Source\\" + filename)), std::istreambuf_iterator<char>());
	}
	return std::string(std::istreambuf_iterator<char>(std::ifstream("Shaders\\" + filename)), std::istreambuf_iterator<char>());
}
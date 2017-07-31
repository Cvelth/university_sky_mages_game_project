#include "OpenGLGraphicsEngine.hpp"
#include "InnerOpenGLGraphicsEngine.hpp"
#include "OpenGL_Mirror\FunctionsMirror\FunctionsMirror.hpp"

#include "GameLogicEngine\GameMap.hpp"
#include "GameLogicEngine\GameCamera.hpp"
#include "GameLogicEngine\AbstractBlock.hpp"
#include "Graphics\RenderInfo\RenderInfo.hpp"

void InnerOpenGLGraphicsEngine::resize(GameCamera* camera) {
	auto size = getSize();
	resize(size.w, size.h, camera);
}
void InnerOpenGLGraphicsEngine::resize(int width, int height, GameCamera* camera) {
	m_aspectRatio = float(width) / height;
	mgl::setViewport(0, 0, width, height);

	if (m_projection) delete m_projection;
	m_projection = new mgl::math::Matrix(mgl::math::Matrix::orthographicMatrix(
		camera->minX_f() * (m_aspectRatio < 1.f ? m_aspectRatio : 1.f),
		camera->maxX_f() * (m_aspectRatio < 1.f ? m_aspectRatio : 1.f),
		camera->maxY_f() * (1.f / (m_aspectRatio < 1.f ? 1.f : m_aspectRatio)),
		camera->minY_f() * (1.f / (m_aspectRatio < 1.f ? 1.f : m_aspectRatio)),
		-1.f,
		+1.f
	));
}

std::string readShader(std::string name);
void OpenGLGraphicsEngine::initializeMapRendering(GameCamera* camera) {
	m_camera = camera;
	for (auto it : m_camera->map()->get_blocks_data())
		it->renderInfo()->get()->send(mgl::DataUsage::StaticDraw);
	m_camera->map()->get(0, 0)->renderInfo()->get()->send(mgl::DataUsage::StaticDraw);

	std::string source = readShader("MapVertexShader.glsl");
	auto map_shader = mgl::Shader::compileShaderSource(mgl::ShaderType::Vertex, source);
	m_map_program = m_engine->linkProgramWithDefaultFragmentShader(map_shader);
	delete map_shader;

	m_map_program->use();
	m_map_program->enableAttrib("position", 4, 8, 0);
	m_map_program->enableAttrib("color", 4, 8, 4);

	m_translation = m_map_program->getUniform("translation");
	m_scaling = m_map_program->getUniform("scaling");
	m_projection = m_map_program->getUniform("projection");

	m_engine->resize(m_camera);

	m_map_program->sendUniform(m_translation, mgl::math::Vector(0.f, 0.f, 0.f, 0.f));
	m_map_program->sendUniform(m_scaling, mgl::math::Vector(1.f, 1.f, 1.f, 1.f));
	m_map_program->sendUniform(m_projection, *m_engine->projection());

	m_engine->setClearColor(0.6f, 0.85f, 0.9f);
}

void OpenGLGraphicsEngine::renderMap() {
	m_engine->clearWindow();

	auto minX = m_camera->minX_i();
	auto maxX = m_camera->maxX_i();
	auto minY = m_camera->minY_i();
	auto maxY = m_camera->maxY_i();
	for (unsigned int x = minX; x < maxX; x++)
		for (unsigned int y = minY; y < maxY; y++) {
			m_map_program->sendUniform(m_translation, mgl::math::Vector(float(x), float(y), 0.f, 0.f));
			m_camera->map()->get(x, y)->renderInfo()->get()->draw();
		}
}

void OpenGLGraphicsEngine::cleanMapRendering() {
	for (auto it : m_camera->map()->get_blocks_data())
		it->renderInfo()->get()->clean();

	if (m_translation) delete m_translation;
	if (m_scaling) delete m_scaling;
	if (m_projection) delete m_projection;
	if (m_map_program) delete m_map_program;
}

#include "Exceptions\OpenGLExceptions.hpp"
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
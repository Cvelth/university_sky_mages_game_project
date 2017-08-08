#include "MyGraphicsLibraryEngine.hpp"
#include "MGL_Window.hpp"
#include "RenderTools\HUD_RenderInfo.hpp"
#include "OpenGL\FunctionsMirror\FunctionsMirror.hpp"
#include "Primitive\AbstractDrawableObject.hpp"

void MyGraphicsLibraryEngine::initializeHUDRendering() {
	m_hud = new HUD_RenderInfo();

	m_HUD_program.program = m_window->linkProgramWithDefaultFragmentShader(
		mgl::Shader::compileShaderSource(mgl::ShaderType::Vertex,
										 readShader("HUDVertexShader.glsl").c_str()));
	m_HUD_program->use();

	m_hud->get()->send(mgl::DataUsage::StaticDraw);
	m_hud->get()->insertVertexArray(m_HUD_program->getVertexArray());

	m_HUD_program->enableAttrib("position", 4, 8, 0);
	m_HUD_program->enableAttrib("color", 4, 8, 4);
	
	m_HUD_program.translation = m_HUD_program->getUniform("translation");
	m_HUD_program.scaling = m_HUD_program->getUniform("scaling");
	m_HUD_program.projection = m_HUD_program->getUniform("projection");

	m_HUD_program->sendUniform(m_HUD_program.translation, mgl::math::Vector(0.f, 0.f, 0.f, 0.f));
	m_HUD_program->sendUniform(m_HUD_program.scaling, mgl::math::Vector(1.f, 1.f, 1.f, 1.f));
	m_HUD_program->sendUniform(m_HUD_program.projection, 
							   mgl::math::Matrix::orthographicMatrix(0, 1, 1, 0, 1, -1));
}

void MyGraphicsLibraryEngine::renderHUD() {
	m_HUD_program->use();
	m_hud->get()->draw();
}

void MyGraphicsLibraryEngine::cleanHUDRendering() {
	m_hud->get()->clean();

	if (m_HUD_program.translation) delete m_HUD_program.translation;
	if (m_HUD_program.scaling) delete m_HUD_program.scaling;
	if (m_HUD_program.projection) delete m_HUD_program.projection;
	if (!m_HUD_program) delete *m_HUD_program;
}
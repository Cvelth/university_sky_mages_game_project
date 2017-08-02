#pragma once
#include "Graphics\AbstractGraphicsEngine\AbstractGraphicsEngine.hpp"

class InnerOpenGLGraphicsEngine;
namespace mgl {
	class ShaderProgram;
	class ShaderVariable;
}

class OpenGLGraphicsEngine : public AbstractGraphicsEngine {
private:
	InnerOpenGLGraphicsEngine *m_engine;

	mgl::ShaderProgram *m_map_program, *m_queue_program;
	mgl::ShaderVariable *m_translation, *m_scaling, *m_projection;
protected:
public:
	OpenGLGraphicsEngine();
	~OpenGLGraphicsEngine();

	virtual void initialize() override;
	virtual void clean() override;

	virtual void initializeWindow(char* title, size_t width, size_t height, bool isFullscreen) override;
	virtual void destroyWindow() override;
	virtual bool isWindowClosed() override;

	virtual void update() override;
	virtual void pollEvents() override;

	virtual void insertController(GameControllerInterface* controller) override;

	virtual void initializeMapRendering(GameCamera* camera) override;
	virtual void renderMap() override;
	virtual void cleanMapRendering() override;

	virtual void initializeQueueRendering() override;
	virtual void renderQueue() override;
	virtual void cleanQueueRendering() override;
};
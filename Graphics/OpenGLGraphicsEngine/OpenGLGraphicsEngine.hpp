#pragma once
#include "Graphics\AbstractGraphicsEngine\AbstractGraphicsEngine.hpp"

class InnerOpenGLGraphicsEngine;
namespace mgl {
	class ShaderProgram;
	class ShaderVariable;
}
struct AbstractProgramStruct {
	mgl::ShaderProgram *program;
	bool operator!() const {
		return program != nullptr;
	}
	mgl::ShaderProgram* operator*() {
		return program;
	}
	const mgl::ShaderProgram* operator*() const {
		return program;
	}
	mgl::ShaderProgram* operator->() {
		return program;
	}
	mgl::ShaderProgram* operator->() const {
		return program;
	}
};
struct MapProgram : public AbstractProgramStruct {	
	mgl::ShaderVariable *translation, *scaling, *projection;
};
struct QueueProgram : public AbstractProgramStruct {
	mgl::ShaderVariable *translation, *scaling, *projection;
};

class OpenGLGraphicsEngine : public AbstractGraphicsEngine {
private:
	InnerOpenGLGraphicsEngine *m_engine;
	MapProgram m_map_program;
	QueueProgram m_queue_program;
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
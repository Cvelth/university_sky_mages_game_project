#pragma once
#include "Graphics\AbstractGraphicsEngine\AbstractGraphicsEngine.hpp"

class InnerOpenGLGraphicsEngine;

class OpenGLGraphicsEngine : public AbstractGraphicsEngine {
private:
	InnerOpenGLGraphicsEngine* m_engine;
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

	virtual void initializeMapRendering(GameMap* map) override;
	virtual void renderMap(GameCamera* camera) override;
	virtual void cleanMapRendering() override;
};
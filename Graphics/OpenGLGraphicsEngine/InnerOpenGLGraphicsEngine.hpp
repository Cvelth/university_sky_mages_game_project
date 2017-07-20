#pragma once
#include "OpenGLGraphicsEngine.hpp"
#include "MyOpenGL\AbstractWindow.hpp"

class InnerOpenGLGraphicsEngine : public mgl::AbstractWindow {
	friend OpenGLGraphicsEngine;
public:
	InnerOpenGLGraphicsEngine();
	~InnerOpenGLGraphicsEngine();

	virtual void initializeRenderProcess() override;
	virtual void renderProcess() override;
	virtual void clearRenderProcess() override;
};


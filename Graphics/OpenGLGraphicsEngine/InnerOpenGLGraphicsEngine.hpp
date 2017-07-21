#pragma once
#include "OpenGLGraphicsEngine.hpp"

#define MGL_DEFAULT_INCLUDE
#include "MyOpenGL.hpp"

class InnerOpenGLGraphicsEngine : public mgl::Window {
	friend OpenGLGraphicsEngine;
public:
	InnerOpenGLGraphicsEngine();
	~InnerOpenGLGraphicsEngine();

	virtual void initializeRenderProcess() override;
	virtual void renderProcess() override;
	virtual void clearRenderProcess() override;
};
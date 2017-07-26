#pragma once
#include "OpenGLGraphicsEngine.hpp"

#define MGL_DEFAULT_INCLUDE
#include "MyOpenGL.hpp"

class InnerOpenGLGraphicsEngine : public mgl::Window {
	friend OpenGLGraphicsEngine;
public:
	InnerOpenGLGraphicsEngine();
	~InnerOpenGLGraphicsEngine();

	virtual void resize(int width, int height) override;

	virtual void initializeRenderProcess() override;
	virtual void renderProcess() override;
	virtual void clearRenderProcess() override;

	inline mgl::math::Matrix* projection() {
		return m_projection;
	}

	unsigned int a;
	void test();
	void testD();
};
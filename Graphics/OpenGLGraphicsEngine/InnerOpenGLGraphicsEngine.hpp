#pragma once
#include "OpenGLGraphicsEngine.hpp"

#define MGL_DEFAULT_INCLUDE
#include "MyOpenGL.hpp"

class InnerOpenGLGraphicsEngine : public mgl::Window {
	friend OpenGLGraphicsEngine;
protected:
	virtual void resize(int width, int height) override;
public:
	InnerOpenGLGraphicsEngine();
	~InnerOpenGLGraphicsEngine();

	virtual void resize(int width, int height, GameCamera* camera);
	virtual void resize(GameCamera* camera);

	virtual void initializeRenderProcess() override;
	virtual void renderProcess() override;
	virtual void clearRenderProcess() override;

	inline mgl::math::Matrix* projection() {
		return m_projection;
	}
};
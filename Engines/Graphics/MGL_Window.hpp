#pragma once
#include "MyGraphicsLibraryEngine.hpp"

#define MGL_DEFAULT_INCLUDE
#include "MGL\MGL.hpp"

class MGLWindow : public mgl::Window {
	friend MyGraphicsLibraryEngine;
public:
	MGLWindow() {}
	~MGLWindow() {}

	virtual void resize(int width, int height, Camera* camera);
	virtual void resize(Camera* camera);

	virtual void initializeRenderProcess() override {}
	virtual void renderProcess() override {}
	virtual void clearRenderProcess() override {}

	inline mgl::math::Matrix* projection() {
		return m_projection;
	}
};
#pragma once
#include "Graphics\AbstractGraphicsEngine\AbstractGraphicsEngine.hpp"

class OpenGLGraphicsEngine : public AbstractGraphicsEngine {
private:

protected:
	virtual void setWindowSettings() override;
public:
	virtual void initialize() override;
	virtual void clean() override;

	virtual void initializeRenderProcess() override;
	virtual void renderProcess() override;
	virtual void clearRenderProcess() override;
public:
	OpenGLGraphicsEngine();
	~OpenGLGraphicsEngine();
};
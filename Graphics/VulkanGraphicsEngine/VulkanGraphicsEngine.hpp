#pragma once
#include "Graphics\AbstractGraphicsEngine\AbstractGraphicsEngine.hpp"

class InnerVulkanGraphicsEngine;

class VulkanGraphicsEngine : public AbstractGraphicsEngine {
private:
	InnerVulkanGraphicsEngine *m_engine;
	bool m_isInnerEngineInitialized;

public:
	VulkanGraphicsEngine();
	~VulkanGraphicsEngine();

	virtual void initialize() override;
	virtual void clean() override;

	virtual void initializeWindow(char* title, size_t width, size_t height, bool isFullscreen) override;
	virtual void destroyWindow() override;
	virtual bool isWindowClosed() override;

	virtual void initializeRenderProcess() override;
	virtual void renderProcess() override;
	virtual void clearRenderProcess() override;
};
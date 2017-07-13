#pragma once
#include "AbstractGraphicsEngine.hpp"

#include "vulkan.h"
#include "glfw3.h"

#include <cstdint>
#include <vector>

struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;
	inline bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

class VulkanGraphicsEngine : public AbstractGraphicsEngine {
private:
	bool isValidationEnabled;
	std::vector<const char*> validationLayers;

	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue queue;
	VkSurfaceKHR surface;
protected:
	VkInstance generateVulkanInstance(); 
	std::vector<const char*> getRequiredGLFWExtensions();
	void insertCallbacks();
	void destroyCallbacks();
	VkSurfaceKHR generateSurface();
	VkPhysicalDevice pickGraphicalDevice();
	VkDevice generateLogicalDevice();
	VkQueue getDeviceQueue();

	static bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
	static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);
	static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

	virtual void setWindowSettings() override;
public:
	~VulkanGraphicsEngine();

	virtual void initialize() override;
	virtual void clean() override;

	virtual void destroyWindow() override;

	virtual void initializeRenderProcess() override;
	virtual void renderProcess() override;
	virtual void clearRenderProcess() override;
};
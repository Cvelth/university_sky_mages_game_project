#pragma once
#include "Graphics\AbstractGraphicsEngine\AbstractGraphicsEngine.hpp"

#include "vulkan\vulkan.h"
#include "glfw\glfw3.h"

#include <cstdint>
#include <vector>

struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;
	inline bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
	inline bool isSuported() {
		return !formats.empty() && !presentModes.empty();
	}
};

struct SwapChainHandle {
	VkSwapchainKHR swapChain;
	std::vector<VkImage> images;

	VkFormat imageFormat;
	VkExtent2D extent;

	VkSwapchainKHR& operator*() {
		return swapChain;
	}
};

struct PipelineHandle {
	VkPipelineLayout layout;
	VkPipeline pipeline;

	VkPipeline& operator*() {
		return pipeline;
	}
};

class VulkanGraphicsEngine : public AbstractGraphicsEngine {
private:
	static bool isValidationEnabled;
	static std::vector<const char*> validationLayers;

	bool m_isInitialized;
	std::vector<const char*> m_deviceExtensions;

	VkInstance m_instance;
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_device;
	VkQueue m_queue;
	VkSurfaceKHR m_surface;
	SwapChainHandle m_swapChain;
	std::vector<VkImageView> m_images;
	VkRenderPass m_renderPass;
	PipelineHandle m_pipeline;
	std::vector<VkFramebuffer> m_framebuffers;
	VkCommandPool m_commandPool;
protected:
	static VkInstance generateVulkanInstance();
	static std::vector<const char*> getRequiredGLFWExtensions(); 
	static void insertCallbacks(VkInstance instance);
	static void destroyCallbacks(VkInstance instance);
	static VkSurfaceKHR generateSurface(VkInstance instance, GLFWwindow* window);
	static VkPhysicalDevice pickGraphicalDevice(VkInstance instance, VkSurfaceKHR surface, std::vector<const char*> deviceExtensions);
	static VkDevice generateLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkQueue queue, std::vector<const char*> deviceExtensions);
	static VkQueue getDeviceQueue(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface);
	static SwapChainHandle generateSwapChain(GLFWwindow* window, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface);
	static std::vector<VkImageView> getSwapChainImages(VkDevice device, SwapChainHandle swapChain);
	static PipelineHandle generateGraphicsPipeline(VkDevice device, SwapChainHandle swapChain, VkRenderPass renderPass);
	static VkRenderPass generateRenderPass(VkDevice device, SwapChainHandle swapChain);
	static std::vector<VkFramebuffer> generateFramebuffers(VkDevice device, SwapChainHandle swapChain, std::vector<VkImageView> images, VkRenderPass renderPass);
	static VkCommandPool generateCommandPool(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface);

	static bool checkValidationLayersSupport();
	static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*> deviceExtensions);
	static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	static bool checkDeviceExtentionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensions);
	static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
	static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, size_t width, size_t height);
	static std::vector<char> readFile(const std::string& filename);
	static VkShaderModule generateShaderModule(const std::vector<char>& code, VkDevice device);

	virtual void setWindowSettings() override;
public:
	VulkanGraphicsEngine();
	~VulkanGraphicsEngine();

	virtual void initialize() override;
	virtual void clean() override;

	virtual void initializeRenderProcess() override;
	virtual void renderProcess() override;
	virtual void clearRenderProcess() override;
};
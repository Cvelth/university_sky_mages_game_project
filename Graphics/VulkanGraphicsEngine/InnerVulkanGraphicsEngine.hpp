#pragma once
#include "VulkanGraphicsEngine.hpp"
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

struct ShaderFilenames {
	char* vertex;
	char* fragment;
	ShaderFilenames() {}
	ShaderFilenames(char* vertex, char* fragment) : vertex(vertex), fragment(fragment) {}
};

class InnerVulkanGraphicsEngine {
	friend VulkanGraphicsEngine;
private:
	static bool isValidationEnabled;
	static std::vector<const char*> validationLayers;

	ShaderFilenames m_shaderFilenames;
	bool m_isInitialized;
	bool m_semaforesWereCreated;
	std::vector<const char*> m_deviceExtensions;

	GLFWwindow* m_window;
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
	std::vector<VkCommandBuffer> m_commandBuffers;

	VkSemaphore m_imageAvailableSemaphore;
	VkSemaphore m_renderFinishedSemaphore;
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
	static PipelineHandle generateGraphicsPipeline(VkDevice device, SwapChainHandle swapChain, VkRenderPass renderPass, ShaderFilenames shaderFilenames);
	static VkRenderPass generateRenderPass(VkDevice device, SwapChainHandle swapChain);
	static std::vector<VkFramebuffer> generateFramebuffers(VkDevice device, SwapChainHandle swapChain, std::vector<VkImageView> images, VkRenderPass renderPass);
	static VkCommandPool generateCommandPool(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface);
	static std::vector<VkCommandBuffer> generateCommandBuffers(VkDevice device, std::vector<VkFramebuffer> framebuffers, VkCommandPool commandPool);
	void generateSemaphores();

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
	static VkSemaphore generateSemaphore(VkDevice device);
public:
	InnerVulkanGraphicsEngine(GLFWwindow* window);
	~InnerVulkanGraphicsEngine();
	
	virtual void initialize();
	virtual void clean();

	virtual void initializeRenderProcess();
	virtual void renderProcess();
	virtual void clearRenderProcess();
};


#include "GameWindow.hpp"
#include "WindowExceptions.hpp"

#include <vector>
#include <functional>

#include "vulkan.h"
#include "glfw3.h"

bool validationEnabledLocal;
VkDebugReportCallbackEXT callback;

void GameWindow::initializeWindow(const char* title) {
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!window)
		throw Exceptions::WindowInitializationException();
}

std::vector<const char*> getRequiredExtensions() {
	std::vector<const char*> extensions;
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	for (unsigned int i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}
	if (validationEnabledLocal) {
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}
	return extensions;
}

#include <iostream>
VkInstance* generateVulkanInstance(const std::vector<const char*> validationLayers = std::vector<const char*>()) {
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "SkyMages";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instInfo = {};
	instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	instInfo.enabledExtensionCount = extensions.size();
	instInfo.ppEnabledExtensionNames = extensions.data();
	if (validationEnabledLocal) {
		instInfo.enabledLayerCount = validationLayers.size();
		instInfo.ppEnabledLayerNames = validationLayers.data();
	} else
		instInfo.enabledLayerCount = 0;

	VkInstance* ret = new VkInstance;
	auto error = vkCreateInstance(&instInfo, nullptr, ret);
	if (error != VK_SUCCESS)
		throw Exceptions::InstanceGenerationException(error);

	return ret;
}

bool checkValidationLayerSupport(const std::vector<const char*> validationLayers) {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data()); 
	
	for (const char* layerName : validationLayers) {
		bool layerFound = false;
		for (const auto& layerProperties : availableLayers) 
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		if (!layerFound) 
			return false;
	}
	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj,
		size_t location, int32_t code, const char* layerPrefix, const char* msg,
		void* userData) {
	throw Exceptions::VulkanDebugCallbackException(code, msg);
	return VK_FALSE;
}

VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
	auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
	if (func != nullptr) 
		return func(instance, pCreateInfo, pAllocator, pCallback);
	else
		return VK_ERROR_EXTENSION_NOT_PRESENT;
}
void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
	if (func != nullptr) {
		func(instance, callback, pAllocator);
	}
}

void insertCallback(VkInstance* instance) {
	if (!validationEnabledLocal) return;

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;

	auto error = CreateDebugReportCallbackEXT(*instance, &createInfo, nullptr, &callback);
	if (error != VK_SUCCESS) {
		throw Exceptions::VulkanDebugCallbackInitializationException(error);
	}
}

bool isDeviceSuitable(VkPhysicalDevice device) {
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	return //deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		deviceFeatures.geometryShader;
}

VkPhysicalDevice pickGraphicalDevice(VkInstance* instance) {
	VkPhysicalDevice ret = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);

	if (deviceCount == 0)
		throw Exceptions::NoSupportedGPUException();

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());

	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			ret = device;
			break;
		}
	}
	if (ret == VK_NULL_HANDLE) {
		throw Exceptions::NoSupportedGPUException();
	}

	return ret;
}

void GameWindow::initializeGraphics() {
#ifdef _DEBUG
	validationEnabledLocal = validationEnabled = true;
#else
	validationEnabledLocal = validationEnabled = false;
#endif
	
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	if(validationEnabled && !checkValidationLayerSupport(validationLayers)) 
		throw Exceptions::LayersNotAvailableException();

	instance = generateVulkanInstance(validationLayers);
	insertCallback(instance);
	pickGraphicalDevice(instance);
}

void GameWindow::cleanGraphics() {
	DestroyDebugReportCallbackEXT(*instance, callback, nullptr);
	vkDestroyInstance(*instance, nullptr);
}

void GameWindow::initializeRenderProcess() {

}

void GameWindow::renderProcess() {
	glfwPollEvents();
}

void GameWindow::clearRenderProcess() {

}

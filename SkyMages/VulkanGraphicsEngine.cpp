#include "VulkanGraphicsEngine.hpp"
#include "WindowExceptions.hpp"

VulkanGraphicsEngine::~VulkanGraphicsEngine() {}

void VulkanGraphicsEngine::setWindowSettings() {
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void VulkanGraphicsEngine::initialize() {
#ifdef _DEBUG
	isValidationEnabled = true;
#else
	isValidationEnabled = false;
#endif

	validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	if (isValidationEnabled && !checkValidationLayerSupport(validationLayers))
		throw Exceptions::LayersNotAvailableException();

	instance = generateVulkanInstance();
	insertCallbacks();
	surface = generateSurface();
	physicalDevice = pickGraphicalDevice();
	device = generateLogicalDevice();
	queue = getDeviceQueue();
}

void VulkanGraphicsEngine::clean() {
	destroyCallbacks();
	vkDestroyDevice(device, nullptr); //Destroys VkQueue as well.
	vkDestroyInstance(instance, nullptr); //Destroys VkPhysicalDevice as well.
}

void VulkanGraphicsEngine::destroyWindow() {
	glfwDestroyWindow(window);
}

void VulkanGraphicsEngine::initializeRenderProcess() {

}

void VulkanGraphicsEngine::renderProcess() {
	glfwPollEvents();
}

void VulkanGraphicsEngine::clearRenderProcess() {

}

bool VulkanGraphicsEngine::checkValidationLayerSupport(const std::vector<const char*>& validationLayers) {
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

VkInstance VulkanGraphicsEngine::generateVulkanInstance() {
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

	auto extensions = getRequiredGLFWExtensions();
	instInfo.enabledExtensionCount = extensions.size();
	instInfo.ppEnabledExtensionNames = extensions.data();
	if (isValidationEnabled) {
		instInfo.enabledLayerCount = validationLayers.size();
		instInfo.ppEnabledLayerNames = validationLayers.data();
	} else
		instInfo.enabledLayerCount = 0;

	VkInstance ret;
	auto error = vkCreateInstance(&instInfo, nullptr, &ret);
	if (error != VK_SUCCESS)
		throw Exceptions::InstanceGenerationException(error);

	return ret;
}

std::vector<const char*> VulkanGraphicsEngine::getRequiredGLFWExtensions() {
	std::vector<const char*> extensions;
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	for (unsigned int i = 0; i < glfwExtensionCount; i++)
		extensions.push_back(glfwExtensions[i]);
	if (isValidationEnabled)
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	return extensions;
}

VkSurfaceKHR VulkanGraphicsEngine::generateSurface() {
	VkSurfaceKHR ret;
	auto error = glfwCreateWindowSurface(instance, window, nullptr, &ret);
	if (error != VK_SUCCESS)
		throw Exceptions::SurfaceInitializationException(error);
	return ret;
}

VkPhysicalDevice VulkanGraphicsEngine::pickGraphicalDevice() {
	VkPhysicalDevice ret = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
		throw Exceptions::NoSupportedGPUException();

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices)
		if (isDeviceSuitable(device, surface)) {
			ret = device;
			break;
		}
	if (ret == VK_NULL_HANDLE)
		throw Exceptions::NoSupportedGPUException();

	return ret;
}

bool VulkanGraphicsEngine::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	return deviceFeatures.geometryShader && findQueueFamilies(device, surface).isComplete();
}

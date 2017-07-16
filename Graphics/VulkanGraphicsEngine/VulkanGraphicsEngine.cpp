#include "VulkanGraphicsEngine.hpp"
#include "Exceptions\WindowExceptions.hpp"

#ifdef _DEBUG
bool VulkanGraphicsEngine::isValidationEnabled = true;
#else
bool VulkanGraphicsEngine::isValidationEnabled = false;
#endif

std::vector<const char*> VulkanGraphicsEngine::validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

VulkanGraphicsEngine::VulkanGraphicsEngine() : isInitialized(false) {}

VulkanGraphicsEngine::~VulkanGraphicsEngine() {}

void VulkanGraphicsEngine::setWindowSettings() {
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void VulkanGraphicsEngine::initialize() {
	deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	if (isValidationEnabled && !checkValidationLayersSupport())
		throw Exceptions::LayersNotAvailableException();

	instance = generateVulkanInstance();
	insertCallbacks(instance);
	surface = generateSurface(instance, window);
	physicalDevice = pickGraphicalDevice(instance, surface, deviceExtensions);
	device = generateLogicalDevice(physicalDevice, surface, queue, deviceExtensions);
	queue = getDeviceQueue(physicalDevice, device, surface);
	swapChain = generateSwapChain(window, physicalDevice, device, surface);
	images = getSwapChainImages(device, swapChain);
	renderPass = generateRenderPass(device, swapChain);
	pipeline = generateGraphicsPipeline(device, swapChain, renderPass);

	isInitialized = true;
}

void VulkanGraphicsEngine::clean() {
	if (isInitialized) {
		vkDestroyPipeline(device, *pipeline, nullptr);
		vkDestroyRenderPass(device, renderPass, nullptr);
		vkDestroyPipelineLayout(device, pipeline.layout, nullptr);
		for (auto it : images)
			vkDestroyImageView(device, it, nullptr);
		vkDestroySwapchainKHR(device, *swapChain, nullptr);
		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyDevice(device, nullptr); //Destroys VkQueue as well.
		destroyCallbacks(instance);
		vkDestroyInstance(instance, nullptr); //Destroys VkPhysicalDevice as well.
	}
}

bool VulkanGraphicsEngine::checkValidationLayersSupport() {
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

VkSurfaceKHR VulkanGraphicsEngine::generateSurface(VkInstance instance, GLFWwindow* window) {
	VkSurfaceKHR ret;
	auto error = glfwCreateWindowSurface(instance, window, nullptr, &ret);
	if (error != VK_SUCCESS)
		throw Exceptions::SurfaceInitializationException(error);
	return ret;
}

VkPhysicalDevice VulkanGraphicsEngine::pickGraphicalDevice(VkInstance instance, VkSurfaceKHR surface, std::vector<const char*> deviceExtensions) {
	VkPhysicalDevice ret = VK_NULL_HANDLE;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
		throw Exceptions::NoSupportedGPUException();

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices)
		if (isDeviceSuitable(device, surface, deviceExtensions)) {
			ret = device;
			break;
		}
	if (ret == VK_NULL_HANDLE)
		throw Exceptions::NoSupportedGPUException();

	return ret;
}

bool VulkanGraphicsEngine::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*> deviceExtensions) {
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	if (false)
		return false;

	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	if (!deviceFeatures.geometryShader)
		return false;

	QueueFamilyIndices queueFamilies = findQueueFamilies(device, surface);
	if (!queueFamilies.isComplete())
		return false;

	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, surface);
	if (!checkDeviceExtentionSupport(device, deviceExtensions) || !swapChainSupport.isSuported())
		return false;

	return true;
}
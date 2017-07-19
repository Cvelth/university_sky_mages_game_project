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

VulkanGraphicsEngine::VulkanGraphicsEngine() : m_isInitialized(false) {}

VulkanGraphicsEngine::~VulkanGraphicsEngine() {}

void VulkanGraphicsEngine::setWindowSettings() {
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void VulkanGraphicsEngine::initialize() {
	m_deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	if (isValidationEnabled && !checkValidationLayersSupport())
		throw Exceptions::LayersNotAvailableException();

	m_instance = generateVulkanInstance();
	insertCallbacks(m_instance);
	m_surface = generateSurface(m_instance, m_window);
	m_physicalDevice = pickGraphicalDevice(m_instance, m_surface, m_deviceExtensions);
	m_device = generateLogicalDevice(m_physicalDevice, m_surface, m_queue, m_deviceExtensions);
	m_queue = getDeviceQueue(m_physicalDevice, m_device, m_surface);
	m_swapChain = generateSwapChain(m_window, m_physicalDevice, m_device, m_surface);
	m_images = getSwapChainImages(m_device, m_swapChain);
	m_renderPass = generateRenderPass(m_device, m_swapChain);
	m_pipeline = generateGraphicsPipeline(m_device, m_swapChain, m_renderPass);

	m_isInitialized = true;
}

void VulkanGraphicsEngine::clean() {
	if (m_isInitialized) {
		vkDestroyPipeline(m_device, *m_pipeline, nullptr);
		vkDestroyRenderPass(m_device, m_renderPass, nullptr);
		vkDestroyPipelineLayout(m_device, m_pipeline.layout, nullptr);
		for (auto it : m_images)
			vkDestroyImageView(m_device, it, nullptr);
		vkDestroySwapchainKHR(m_device, *m_swapChain, nullptr);
		vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
		vkDestroyDevice(m_device, nullptr); //Destroys VkQueue as well.
		destroyCallbacks(m_instance);
		vkDestroyInstance(m_instance, nullptr); //Destroys VkPhysicalDevice as well.
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
	instInfo.enabledExtensionCount = (uint32_t) extensions.size();
	instInfo.ppEnabledExtensionNames = extensions.data();
	if (isValidationEnabled) {
		instInfo.enabledLayerCount = (uint32_t) validationLayers.size();
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
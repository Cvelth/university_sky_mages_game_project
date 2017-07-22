#include "InnerVulkanGraphicsEngine.hpp"
#include "Exceptions\VulkanExceptions.hpp"

#ifdef _DEBUG
bool InnerVulkanGraphicsEngine::isValidationEnabled = true;
#else
bool InnerVulkanGraphicsEngine::isValidationEnabled = false;
#endif

std::vector<const char*> InnerVulkanGraphicsEngine::validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};

InnerVulkanGraphicsEngine::InnerVulkanGraphicsEngine(GLFWwindow * window) : m_window(window), m_isInitialized(false), m_semaforesWereCreated(false) {}

InnerVulkanGraphicsEngine::~InnerVulkanGraphicsEngine() {
	clean();
}

void InnerVulkanGraphicsEngine::initialize() {
	m_deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	if (isValidationEnabled && !checkValidationLayersSupport())
		throw Exceptions::LayersNotAvailableException();

	//m_shaderFilenames = ShaderFilenames("Circle.vk.vert.spv", "CoordinateColor.vk.frag.spv");
	m_shaderFilenames = ShaderFilenames("Triangle.vert.vk.spv", "Triangle.frag.vk.spv");

	m_instance = generateVulkanInstance();
	insertCallbacks(m_instance);
	m_surface = generateSurface(m_instance, m_window);
	m_physicalDevice = pickGraphicalDevice(m_instance, m_surface, m_deviceExtensions);
	m_device = generateLogicalDevice(m_physicalDevice, m_surface, m_queue, m_deviceExtensions);
	m_queue = getDeviceQueue(m_physicalDevice, m_device, m_surface);
	m_swapChain = generateSwapChain(m_window, m_physicalDevice, m_device, m_surface);
	m_images = getSwapChainImages(m_device, m_swapChain);
	m_renderPass = generateRenderPass(m_device, m_swapChain);
	m_pipeline = generateGraphicsPipeline(m_device, m_swapChain, m_renderPass, m_shaderFilenames);
	m_framebuffers = generateFramebuffers(m_device, m_swapChain, m_images, m_renderPass);
	m_commandPool = generateCommandPool(m_physicalDevice, m_device, m_surface);
	m_commandBuffers = generateCommandBuffers(m_device, m_framebuffers, m_commandPool);
	generateSemaphores();

	m_isInitialized = true;
}

void InnerVulkanGraphicsEngine::clean() {
	if (m_isInitialized) {
		if (m_semaforesWereCreated) {
			vkDestroySemaphore(m_device, m_imageAvailableSemaphore, nullptr);
			vkDestroySemaphore(m_device, m_renderFinishedSemaphore, nullptr);
		}
		vkDestroyCommandPool(m_device, m_commandPool, nullptr); //Destroys all the VkCommandBuffer's as well.
		for (auto it : m_framebuffers)
			vkDestroyFramebuffer(m_device, it, nullptr);
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

bool InnerVulkanGraphicsEngine::checkValidationLayersSupport() {
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

VkInstance InnerVulkanGraphicsEngine::generateVulkanInstance() {
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

std::vector<const char*> InnerVulkanGraphicsEngine::getRequiredGLFWExtensions() {
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

VkSurfaceKHR InnerVulkanGraphicsEngine::generateSurface(VkInstance instance, GLFWwindow* window) {
	VkSurfaceKHR ret;
	auto error = glfwCreateWindowSurface(instance, window, nullptr, &ret);
	if (error != VK_SUCCESS)
		throw Exceptions::SurfaceInitializationException(error);
	return ret;
}

VkPhysicalDevice InnerVulkanGraphicsEngine::pickGraphicalDevice(VkInstance instance, VkSurfaceKHR surface, std::vector<const char*> deviceExtensions) {
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

bool InnerVulkanGraphicsEngine::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface, const std::vector<const char*> deviceExtensions) {
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

VkSemaphore InnerVulkanGraphicsEngine::generateSemaphore(VkDevice device) {
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkSemaphore ret;
	auto error = vkCreateSemaphore(device, &semaphoreInfo, nullptr, &ret);
	if (error != VK_SUCCESS)
		throw Exceptions::SemaphoreGenerationException(error);

	return ret;
}

PipelineHandle InnerVulkanGraphicsEngine::generateGraphicsPipeline(VkDevice device, SwapChainHandle swapChain, VkRenderPass renderPass, ShaderFilenames shaderFilenames) {
	auto vertexShader = generateShaderModule(readFile(shaderFilenames.vertex), device);
	auto fragmentShader = generateShaderModule(readFile(shaderFilenames.fragment), device);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertexShader;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragmentShader;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) swapChain.extent.width;
	viewport.height = (float) swapChain.extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = swapChain.extent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pushConstantRangeCount = 0;

	PipelineHandle ret;
	{
		auto error = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &ret.layout);
		if (error != VK_SUCCESS) {
			throw Exceptions::PipelineCreationException(error);
		}
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional
	pipelineInfo.layout = ret.layout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	{
		auto error = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &*ret);
		if (error != VK_SUCCESS)
			throw Exceptions::PipelineCreationException(error);
	}

	vkDestroyShaderModule(device, fragmentShader, nullptr);
	vkDestroyShaderModule(device, vertexShader, nullptr);
	return ret;
}

VkShaderModule InnerVulkanGraphicsEngine::generateShaderModule(const std::vector<char>& code, VkDevice device) {
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	std::vector<uint32_t> codeAligned(code.size() / sizeof(uint32_t) + 1);
	memcpy(codeAligned.data(), code.data(), code.size());
	createInfo.pCode = codeAligned.data();

	VkShaderModule ret;
	auto error = vkCreateShaderModule(device, &createInfo, nullptr, &ret);
	if (error != VK_SUCCESS)
		throw Exceptions::ShaderCreationException(error);

	return ret;
}

VkRenderPass InnerVulkanGraphicsEngine::generateRenderPass(VkDevice device, SwapChainHandle swapChain) {
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChain.imageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef = {};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VkRenderPass ret;
	auto error = vkCreateRenderPass(device, &renderPassInfo, nullptr, &ret);
	if (error != VK_SUCCESS)
		throw Exceptions::RenderPassCreationException(error);

	return ret;
}

#include <set>

bool isSurfaceSupported(VkPhysicalDevice physicalDevice, unsigned int familyIndex, VkSurfaceKHR surface) {
	VkBool32 presentSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, familyIndex, surface, &presentSupport);
	return presentSupport;
}

QueueFamilyIndices InnerVulkanGraphicsEngine::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
	QueueFamilyIndices ret;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	for (unsigned int i = 0; i < queueFamilies.size() && !ret.isComplete(); i++)
		if (queueFamilies[i].queueCount > 0) {
			if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				ret.graphicsFamily = i;
			if (isSurfaceSupported(physicalDevice, i, surface))
				ret.presentFamily = i;
		}
	return ret;
}

SwapChainSupportDetails InnerVulkanGraphicsEngine::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
	SwapChainSupportDetails ret;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &ret.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0) {
		ret.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, ret.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		ret.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, ret.presentModes.data());
	}

	return ret;
}

VkQueue InnerVulkanGraphicsEngine::getDeviceQueue(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface) {
	auto indices = findQueueFamilies(physicalDevice, surface);
	VkQueue ret;
	vkGetDeviceQueue(device, indices.presentFamily, 0, &ret);
	return ret;
}

VkDevice InnerVulkanGraphicsEngine::generateLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkQueue queue, std::vector<const char*> deviceExtensions) {
	auto indices = findQueueFamilies(physicalDevice, surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

	float queuePriority = 1.0f;
	for (int queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = (uint32_t) queueCreateInfos.size();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = (uint32_t) deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	if (isValidationEnabled) {
		createInfo.enabledLayerCount = (uint32_t) validationLayers.size();
		createInfo.ppEnabledLayerNames = validationLayers.data();
	} else
		createInfo.enabledLayerCount = 0;

	VkDevice ret;
	auto error = vkCreateDevice(physicalDevice, &createInfo, nullptr, &ret);
	if (error != VK_SUCCESS)
		throw Exceptions::LogicalGraphicalDeviceGenerationException(error);

	vkGetDeviceQueue(ret, indices.presentFamily, 0, &queue);

	return ret;
}

bool InnerVulkanGraphicsEngine::checkDeviceExtentionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensions) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : availableExtensions)
		requiredExtensions.erase(extension.extensionName);
	return requiredExtensions.empty();
}

#include <algorithm>

VkSurfaceFormatKHR InnerVulkanGraphicsEngine::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
		return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};

	for (const auto& availableFormat : availableFormats)
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return availableFormat;

	return availableFormats[0];
}

VkPresentModeKHR InnerVulkanGraphicsEngine::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
	for (const auto& availablePresentMode : availablePresentModes)
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			return availablePresentMode;
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			bestMode = availablePresentMode;
	return bestMode;
}

VkExtent2D InnerVulkanGraphicsEngine::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, size_t width, size_t height) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return capabilities.currentExtent;
	else {
		VkExtent2D actualExtent = {(uint32_t) width, (uint32_t) height};
		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
		return actualExtent;
	}
}

SwapChainHandle InnerVulkanGraphicsEngine::generateSwapChain(GLFWwindow* window, VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface) {
	SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		imageCount = swapChainSupport.capabilities.maxImageCount;

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
	uint32_t queueFamilyIndices[] = {(uint32_t) indices.graphicsFamily, (uint32_t) indices.presentFamily};
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	SwapChainHandle ret;
	auto error = vkCreateSwapchainKHR(device, &createInfo, nullptr, &ret.swapChain);
	if (error != VK_SUCCESS)
		throw Exceptions::SwapChainCreationException(error);

	vkGetSwapchainImagesKHR(device, *ret, &imageCount, nullptr);
	ret.images.resize(imageCount);
	vkGetSwapchainImagesKHR(device, *ret, &imageCount, ret.images.data());

	ret.imageFormat = surfaceFormat.format;
	ret.extent = extent;

	return ret;
}

std::vector<VkImageView> InnerVulkanGraphicsEngine::getSwapChainImages(VkDevice device, SwapChainHandle swapChain) {
	std::vector<VkImageView> ret;
	ret.resize(swapChain.images.size());

	for (uint32_t i = 0; i < swapChain.images.size(); i++) {
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChain.images[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChain.imageFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		auto error = vkCreateImageView(device, &createInfo, nullptr, &ret.at(i));
		if (error != VK_SUCCESS)
			throw Exceptions::SwapChainCreationException(error);
	}

	return ret;
}

std::vector<VkFramebuffer> InnerVulkanGraphicsEngine::generateFramebuffers(VkDevice device, SwapChainHandle swapChain, std::vector<VkImageView> images, VkRenderPass renderPass) {
	std::vector<VkFramebuffer> ret;
	ret.resize(images.size());

	for (size_t i = 0; i < ret.size(); i++) {
		VkImageView attachments[] = {
			images.at(i)
		};
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = swapChain.extent.width;
		framebufferInfo.height = swapChain.extent.height;
		framebufferInfo.layers = 1;

		auto error = vkCreateFramebuffer(device, &framebufferInfo, nullptr, &ret.at(i));
		if (error != VK_SUCCESS)
			throw Exceptions::FramebufferGenerationException(error);
	}

	return ret;
}

VkCommandPool InnerVulkanGraphicsEngine::generateCommandPool(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface) {
	VkCommandPool ret;

	QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
	poolInfo.flags = 0; // Optional

	auto error = vkCreateCommandPool(device, &poolInfo, nullptr, &ret);
	if (error != VK_SUCCESS)
		throw Exceptions::CommandPoolGenerationException(error);

	return ret;
}

std::vector<VkCommandBuffer> InnerVulkanGraphicsEngine::generateCommandBuffers(VkDevice device, std::vector<VkFramebuffer> framebuffers, VkCommandPool commandPool) {
	std::vector<VkCommandBuffer> ret;
	ret.resize(framebuffers.size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t) ret.size();

	auto error = vkAllocateCommandBuffers(device, &allocInfo, ret.data());
	if (error != VK_SUCCESS)
		throw Exceptions::CommandPoolGenerationException(error);

	return ret;
}

void InnerVulkanGraphicsEngine::generateSemaphores() {
	m_imageAvailableSemaphore = generateSemaphore(m_device);
	m_renderFinishedSemaphore = generateSemaphore(m_device);
	m_semaforesWereCreated = true;
}

#include <functional>

VkDebugReportCallbackEXT callback;

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
	if (func != nullptr)
		func(instance, callback, pAllocator);
}

void InnerVulkanGraphicsEngine::insertCallbacks(VkInstance instance) {
	if (!isValidationEnabled) return;

	VkDebugReportCallbackCreateInfoEXT createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = debugCallback;

	auto error = CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback);
	if (error != VK_SUCCESS)
		throw Exceptions::VulkanDebugCallbackInitializationException(error);
}

void InnerVulkanGraphicsEngine::destroyCallbacks(VkInstance instance) {
	DestroyDebugReportCallbackEXT(instance, callback, nullptr);
}

#include <fstream>

std::vector<char> InnerVulkanGraphicsEngine::readFile(const std::string& filename) {
	std::ifstream file;
	file.open("Shaders\\" + filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		file.open("..\\Graphics\\Shaders\\Output\\" + filename, std::ios::ate | std::ios::binary);
		if (!file.is_open())
			throw Exceptions::ShaderFileException();
	}

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();
	return buffer;
}
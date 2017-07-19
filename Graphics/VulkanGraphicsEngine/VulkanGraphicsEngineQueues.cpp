#include "VulkanGraphicsEngine.hpp"
#include "Exceptions\VulkanExceptions.hpp"

#include <set>

bool isSurfaceSupported(VkPhysicalDevice physicalDevice, unsigned int familyIndex, VkSurfaceKHR surface) {
	VkBool32 presentSupport = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, familyIndex, surface, &presentSupport);
	return presentSupport;
}

QueueFamilyIndices VulkanGraphicsEngine::findQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
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

SwapChainSupportDetails VulkanGraphicsEngine::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
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

VkQueue VulkanGraphicsEngine::getDeviceQueue(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface) {
	auto indices = findQueueFamilies(physicalDevice, surface);
	VkQueue ret;
	vkGetDeviceQueue(device, indices.presentFamily, 0, &ret);
	return ret;
}

VkDevice VulkanGraphicsEngine::generateLogicalDevice(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkQueue queue, std::vector<const char*> deviceExtensions) {
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

bool VulkanGraphicsEngine::checkDeviceExtentionSupport(VkPhysicalDevice physicalDevice, const std::vector<const char*> deviceExtensions) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
	for (const auto& extension : availableExtensions)
		requiredExtensions.erase(extension.extensionName);
	return requiredExtensions.empty();
}
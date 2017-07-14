#include "VulkanGraphicsEngine.hpp"
#include "WindowExceptions.hpp"

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

VkQueue VulkanGraphicsEngine::getDeviceQueue() {
	auto indices = findQueueFamilies(physicalDevice, surface);
	VkQueue ret;
	vkGetDeviceQueue(device, indices.presentFamily, 0, &ret);
	return ret;
}

VkDevice VulkanGraphicsEngine::generateLogicalDevice() {
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
	createInfo.queueCreateInfoCount = queueCreateInfos.size();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = 0;
	if (isValidationEnabled) {
		createInfo.enabledLayerCount = validationLayers.size();
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
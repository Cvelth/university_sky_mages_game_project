#include "VulkanExceptions.hpp"
#include <iostream>

#define ClassName(name) #name

void Exceptions::AbstractVulkanErrorException::print() const {
	std::cerr << ClassName(this) << " was thrown with code " << m_code << '\n';
	std::cerr << what() << '\n';
}
char const* Exceptions::AbstractVulkanErrorException::what() const {
	switch (m_code) {
		case 0:
			return "No error.";
		case -1:
			return "Host is out of memory.";
		case -2:
			return "Device(GPU) is out of memory.";
		case -3:
			return "Initialization fail.";
		case -4:
			return "Device was lost.";
		case -5:
			return "Memory map has failed.";
		case -6:
			return "Layer isn't present.";
		case -7:
			return "Requested extension isn't present.";
		case -8:
			return "Requested feature isn't present.";
		case -9:
			return "Driver is incompatible.\n"
				"There is a posibility that the GPU doesn't suport Vulkan API";
		case -10:
			return "Too many objects.";
		case -11:
			return "Format isn't supported.";
		case -12:
			return "Pool is fragmented.";
		case -1000000000:
			return "Surface has lost KHR.";
		case -1000000001:
			return "Native vindow is in use.";
		case -1000001004:
			return "KHR is out of date.";
		case -1000003001:
			return "Display is incompatible.";
		case -1000011001:
			return "Validation has failed.";
		case -1000012000:
			return "Invalid shader.";
		case -1000069000:
			return "Out of pool memory.";
		case -1000072003:
			return "Invalid external handle.";
		default:
			return "Unknown error.";
	}
}

void Exceptions::VulkanCallBackException::print() const {
	std::cerr << ClassName(this) << " was thrown with code " << m_code << '\n';
	std::cerr << what() << '\n';
}
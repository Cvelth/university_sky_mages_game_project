#include "WindowExceptions.hpp"
#include <iostream>

void Exceptions::AbstractVulkanErrorException::print() {
	std::cerr << name << " was thrown with code " << error_code << std::endl;
	switch (error_code) {
		case 0:
			std::cerr << "No error." << std::endl;
			break;
		case -1:
			std::cerr << "Host is out of memory." << std::endl;
			break;
		case -2:
			std::cerr << "Device(GPU) is out of memory." << std::endl;
			break;
		case -3:
			std::cerr << "Initialization fail." << std::endl;
			break;
		case -4:
			std::cerr << "Device was lost." << std::endl;
			break;
		case -5:
			std::cerr << "Memory map has failed." << std::endl;
			break;
		case -6:
			std::cerr << "Layer isn't present." << std::endl;
			break;
		case -7:
			std::cerr << "Requested extension isn't present." << std::endl;
			break;
		case -8:
			std::cerr << "Requested feature isn't present." << std::endl;
			break;
		case -9:
			std::cerr << "Driver is incompatible.\n"
				"There is a posibility that the GPU doesn't suport Vulkan API" << std::endl;
			break;
		case -10:
			std::cerr << "Too many objects." << std::endl;
			break;
		case -11:
			std::cerr << "Format isn't supported." << std::endl;
			break;
		case -12:
			std::cerr << "Pool is fragmented." << std::endl;
			break;
		case -1000000000:
			std::cerr << "Surface has lost KHR." << std::endl;
			break;
		case -1000000001:
			std::cerr << "Native vindow is in use." << std::endl;
			break;
		case -1000001004:
			std::cerr << "KHR is out of date." << std::endl;
			break;
		case -1000003001:
			std::cerr << "Display is incompatible." << std::endl;
			break;
		case -1000011001:
			std::cerr << "Validation has failed." << std::endl;
			break;
		case -1000012000:
			std::cerr << "Invalid shader." << std::endl;
			break;
		case -1000069000:
			std::cerr << "Out of pool memory." << std::endl;
			break;
		case -1000072003:
			std::cerr << "Invalid external handle." << std::endl;
			break;
		default:
			std::cerr << "Unknown error." << std::endl;
			break;
	}

}

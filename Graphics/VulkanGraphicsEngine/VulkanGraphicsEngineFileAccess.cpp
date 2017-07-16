#include "VulkanGraphicsEngine.hpp"
#include "Exceptions\WindowExceptions.hpp"
#include <fstream>

std::vector<char> VulkanGraphicsEngine::readFile(const std::string& filename) {
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
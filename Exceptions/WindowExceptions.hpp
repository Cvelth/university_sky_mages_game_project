#pragma once
#include "AbstractExceptions.hpp"
namespace Exceptions {
	class AbstractVulkanErrorException : public AbstractIntegerErrorException {
	public: AbstractVulkanErrorException(char* name, int code)
		: AbstractIntegerErrorException(name, code, "") {}
			void print() override;
	};

	class GLFWInitializationException : public AbstractStringException {
	public: GLFWInitializationException() 
		: AbstractStringException("GLFWInitializationException", "Some error has occured during GLFW intialization.") {}
	};
	class WindowInitializationException : public AbstractStringException {
	public: WindowInitializationException()
		: AbstractStringException("WindowInitializationException", "Some error has occured during Window intialization.") {}
	};
	class GLFWCallBackException : public AbstractIntegerErrorException {
	public: GLFWCallBackException(int code, const char* desc)
		: AbstractIntegerErrorException("GLFWCallBackException", code, desc) {}
	};
	class InstanceGenerationException : public AbstractVulkanErrorException {
	public: InstanceGenerationException(int code)
		: AbstractVulkanErrorException("InstanceGenerationException", code) {}
	};
	class LayersNotAvailableException : public AbstractStringException {
	public: LayersNotAvailableException() 
		: AbstractStringException("LayersNotAvailableException", "Some of the Vulkan validation layers can't be accessed.\n"
		"Try using release build or reinstalling VulkanSDK.") {}
	};
	class VulkanDebugCallbackException : public AbstractIntegerErrorException {
	public: VulkanDebugCallbackException(int code, const char* desc)
		: AbstractIntegerErrorException("VulkanDebugCallbackException", code, desc) {}
	};
	class VulkanDebugCallbackInitializationException : public AbstractIntegerErrorException {
	public: VulkanDebugCallbackInitializationException(int code)
		: AbstractIntegerErrorException("VulkanDebugCallbackInitializationException", code, "") {}
	};
	class NoSupportedGPUException : public AbstractStringException {
	public: NoSupportedGPUException()
		: AbstractStringException("NoSupportedGPUException", "No GPU with Vulkan support was found.") {}
	};
	class LogicalGraphicalDeviceGenerationException : public AbstractVulkanErrorException {
	public: LogicalGraphicalDeviceGenerationException(int code)
		: AbstractVulkanErrorException("LogicalGraphicalDeviceGenerationException", code) {}
	};
	class SurfaceInitializationException : public AbstractVulkanErrorException {
	public: SurfaceInitializationException(int code)
		: AbstractVulkanErrorException("SurfaceInitializationException", code) {}
	};
	class SwapChainCreationException : public AbstractStringException {
	public: SwapChainCreationException()
		: AbstractStringException("SwapChainCreationException", "Some error has occured during SwapChain intialization.") {}
	};
}
#pragma once
#include "WindowExceptions.hpp"

namespace Exceptions {
	class AbstractVulkanErrorException : public AbstractIntegerErrorException {
	public: AbstractVulkanErrorException(char* name, int code)
		: AbstractIntegerErrorException(name, code, "") {}
			void print() override;
	};
	class InstanceGenerationException : public AbstractVulkanErrorException {
	public: InstanceGenerationException(int code)
		: AbstractVulkanErrorException("InstanceGenerationException", code) {}
	};
	class VulkanDebugCallbackException : public AbstractIntegerErrorException {
	public: VulkanDebugCallbackException(int code, const char* desc)
		: AbstractIntegerErrorException("VulkanDebugCallbackException", code, desc) {}
	};
	class VulkanDebugCallbackInitializationException : public AbstractIntegerErrorException {
	public: VulkanDebugCallbackInitializationException(int code)
		: AbstractIntegerErrorException("VulkanDebugCallbackInitializationException", code, "") {}
	};
	class LayersNotAvailableException : public AbstractStringException {
	public: LayersNotAvailableException()
		: AbstractStringException("LayersNotAvailableException", "Some of the Vulkan validation layers can't be accessed.\n"
								  "Try using release build or reinstalling VulkanSDK.") {}
	};
	class NoVulkanSupportedGPUException : public NoSupportedGPUException {
	public: NoVulkanSupportedGPUException()
		: NoSupportedGPUException("NoVulkanSupportedGPUException", "No GPU with Vulkan support was found.") {}
	};
	class LogicalGraphicalDeviceGenerationException : public AbstractVulkanErrorException {
	public: LogicalGraphicalDeviceGenerationException(int code)
		: AbstractVulkanErrorException("LogicalGraphicalDeviceGenerationException", code) {}
	};
	class SurfaceInitializationException : public AbstractVulkanErrorException {
	public: SurfaceInitializationException(int code)
		: AbstractVulkanErrorException("SurfaceInitializationException", code) {}
	};
	class SwapChainCreationException : public AbstractVulkanErrorException {
	public: SwapChainCreationException(int code)
		: AbstractVulkanErrorException("SwapChainCreationException", code) {}
	};
	class ShaderCreationException : public AbstractVulkanErrorException {
	public: ShaderCreationException(int code)
		: AbstractVulkanErrorException("ShaderCreationException", code) {}
	};
	class PipelineCreationException : public AbstractVulkanErrorException {
	public: PipelineCreationException(int code)
		: AbstractVulkanErrorException("PipelineCreationException", code) {}
	};
	class RenderPassCreationException : public AbstractVulkanErrorException {
	public: RenderPassCreationException(int code)
		: AbstractVulkanErrorException("RenderPassCreationException", code) {}
	};
	class FramebufferGenerationException : public AbstractVulkanErrorException {
	public: FramebufferGenerationException(int code)
		: AbstractVulkanErrorException("FramebufferGenerationException", code) {}
	};
	class CommandPoolGenerationException : public AbstractVulkanErrorException {
	public: CommandPoolGenerationException(int code)
		: AbstractVulkanErrorException("CommandPoolGenerationException", code) {}
	};
	class RenderInitializationException : public AbstractVulkanErrorException {
	public: RenderInitializationException(int code)
		: AbstractVulkanErrorException("RenderInitializationException", code) {}
	};
	class SemaphoreGenerationException : public AbstractVulkanErrorException {
	public: SemaphoreGenerationException(int code)
		: AbstractVulkanErrorException("SemaphoreGenerationException", code) {}
	};
	class VulkanRenderException : public AbstractVulkanErrorException {
	public: VulkanRenderException(int code)
		: AbstractVulkanErrorException("VulkanRenderException", code) {}
	};
}
#pragma once
#include "Shared\AbstractException.hpp"

namespace Exceptions {
	class AbstractVulkanErrorException : public AbstractException {
	protected:
		int m_code;
	public:
		AbstractVulkanErrorException(int code) : m_code(code) {}
		virtual void print() const override;
		virtual char const* what() const override;
	};
	class VulkanCallBackException : public AbstractException {
	protected:
		int m_code;
	public:
		VulkanCallBackException(int code, const char* desc) : AbstractException(desc), m_code(code) {}
		virtual void print() const override;
	};
}
#define DefineNewVulkanException(name) namespace Exceptions {class name : public Exceptions::AbstractVulkanErrorException {public: using AbstractVulkanErrorException::AbstractVulkanErrorException;};}

DefineNewException(GraphicEngineInitializationException);
DefineNewVulkanException(VulkanInitializationException);
DefineNewVulkanException(VulkanRenderException);
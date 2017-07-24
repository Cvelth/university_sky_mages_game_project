#pragma once
#include "AbstractExceptions.hpp"
namespace Exceptions {
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
	class NoSupportedGPUException : public AbstractStringException {
	public: NoSupportedGPUException()
		: AbstractStringException("NoSupportedGPUException", "There are no supported GPU in the system.") {}
			NoSupportedGPUException(char* str1, char* str2) : AbstractStringException(str1, str2) {}
	};
	class ShaderFileException : public AbstractStringException {
	public: ShaderFileException()
		: AbstractStringException("ShaderFileException", "The program can't access the shader file.") {}
	};
	class RenderingIsTooSlowException : public AbstractStringException {
	public: RenderingIsTooSlowException()
		: AbstractStringException("RenderingIsTooSlowException", "It seems like you PC isn't capable of mantaining 60 updates per second.") {}
	};
}
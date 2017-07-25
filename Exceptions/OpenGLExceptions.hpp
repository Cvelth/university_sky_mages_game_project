#pragma once
#include "WindowExceptions.hpp"

namespace Exceptions {
	class ShaderCompilationException : public AbstractStringException {
	public: ShaderCompilationException()
		: AbstractStringException("ShaderCompilationException", "Incorrect Shader was inserted.") {}
	};
	class MapShaderCompilationException : public AbstractStringException {
	public: MapShaderCompilationException()
		: AbstractStringException("MapShaderCompilationException", "Incorrect MapVertexShader was inserted.") {}
	};
}
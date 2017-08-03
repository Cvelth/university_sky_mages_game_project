#pragma once
#include "AbstractExceptions.hpp"
namespace Exceptions {
	class FileAccessException : public AbstractStringException {
	public: FileAccessException()
		: AbstractStringException("FileAccessException", "File cannot be open.") {}
	};
}
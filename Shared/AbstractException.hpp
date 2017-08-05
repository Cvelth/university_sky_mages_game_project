#pragma once
#include <exception>
namespace Exceptions {
	class AbstractException : public std::exception {
	public:
		using std::exception::exception;
		virtual void print() const;
	};
}
#define DefineNewException(name) namespace Exceptions {class name : public Exceptions::AbstractException {public: using AbstractException::AbstractException;};}
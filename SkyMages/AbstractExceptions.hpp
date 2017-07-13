#pragma once
namespace Exceptions {
	class AbstractException {
	public:
		AbstractException() {}
		virtual void print();
	};
	class AbstractStringException : public AbstractException {
	protected:
		const char* name;
		const char* desc;
	public:
		AbstractStringException(const char* name, const char* desc) : AbstractException(), name(name), desc(desc) {}
		virtual void print() override;
	};

	class AbstractIntegerErrorException : public AbstractStringException {
	protected:
		int error_code;
	public:
		AbstractIntegerErrorException(const char* name, int code, const char* desc)
			: AbstractStringException(name, desc), error_code(code) {}
		virtual void print() override;
	};
}
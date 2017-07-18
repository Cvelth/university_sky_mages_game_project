#pragma once
namespace Exceptions {
	class AbstractException {
	public:
		AbstractException() {}
		virtual void print();
	};
	class AbstractStringException : public AbstractException {
	protected:
		const char* m_name;
		const char* m_desc;
	public:
		AbstractStringException(const char* name, const char* desc) : AbstractException(), m_name(name), m_desc(desc) {}
		virtual void print() override;
	};

	class AbstractIntegerErrorException : public AbstractStringException {
	protected:
		int m_error_code;
	public:
		AbstractIntegerErrorException(const char* name, int code, const char* desc)
			: AbstractStringException(name, desc), m_error_code(code) {}
		virtual void print() override;
	};
}
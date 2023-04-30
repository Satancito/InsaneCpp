#pragma once
#ifndef INSANE_EXCEPTION_H
#define INSANE_EXCEPTION_H

#include <Insane/Insane.h>
#include <Insane/InsanePreprocessor.h>
#include <memory>
#include <exception>
#include <string>
#define USING_NS_INSANE_EXCEPTION using namespace InsaneIO::Insane::Exception

#define DEFAULT_HEADER_PREFIX_STRING ("██"s)
#define DEFAULT_FOOTER_PREFIX_STRING ("█"s)

namespace InsaneIO::Insane::Exception
{

	class ExceptionBase : public std::exception, IClone<ExceptionBase>
	{
	public:
		ExceptionBase(const String& name, const String& function, const String& file, const int& line, const String& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr);
		ExceptionBase(const ExceptionBase& other);

		virtual String GetErrorMessage() const noexcept;
		String GetName() const;
		int GetErrorCode() const noexcept;
		std::unique_ptr<ExceptionBase> GetInnerException() const noexcept;

		virtual const char* what() const noexcept override;
		String GetStackTrace(const String& tag = EMPTY_STRING, const String& header = DEFAULT_HEADER_PREFIX_STRING, const String& footer = DEFAULT_FOOTER_PREFIX_STRING) const;
	private:
		const String _ErrorMessage;
		const int _ErrorCode;
		const std::unique_ptr<ExceptionBase> _InnerException;
		const String _Name;
	};

	class ParseException : public ExceptionBase
	{
	public:
		using ExceptionBase::ExceptionBase;
		ParseException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr);
		virtual std::unique_ptr<ExceptionBase> Clone() const override;
	private:
	};

	class CryptoException : public ExceptionBase
	{
	public:
		CryptoException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr);
		virtual std::unique_ptr<ExceptionBase> Clone() const override;
	private:
	};

	class ArgumentException : public ExceptionBase
	{
	public:
		ArgumentException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr);
		virtual std::unique_ptr<ExceptionBase> Clone() const override;
	private:
	};

	class NotImplementedException : public ExceptionBase
	{
	public:
		NotImplementedException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr);
		virtual std::unique_ptr<ExceptionBase> Clone() const override;
	private:
	};

	class SerializeException : public ExceptionBase
	{
	public:
		SerializeException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr);
		virtual std::unique_ptr<ExceptionBase> Clone() const override;
	private:
	};

	class DeserializeException : public ExceptionBase
	{
	public:
		DeserializeException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr);
		virtual std::unique_ptr<ExceptionBase> Clone() const override;
	private:
	};

	class AbstractImplementationException : public ExceptionBase
	{
	public:
		AbstractImplementationException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr);
		virtual std::unique_ptr<ExceptionBase> Clone() const override;
	private:
	};

	class ConvertException : public ExceptionBase
	{
	public:
		ConvertException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr);
		virtual std::unique_ptr<ExceptionBase> Clone() const override;
	private:
	};
}
#endif //! INSANE_EXCEPTION_H
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


	class ExceptionBase : public IClone<ExceptionBase>
	{
	public:
		virtual ~ExceptionBase() = default;
		ExceptionBase(const String& name, const String& function, const String& file, const int& line, const String& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr)
			: _Name(name), _ErrorMessage(CreateExceptionMessage(name, function, file, line, message, code)), _ErrorCode(code), _InnerException(innerException ? std::move(innerException) : nullptr)
		{
				std::cout << _ErrorMessage << std::endl;
			
		}

		ExceptionBase(const ExceptionBase& instance)
			:
			_Name(instance._Name),
			_ErrorMessage(instance._ErrorMessage),
			_ErrorCode(instance._ErrorCode),
			_InnerException(instance._InnerException ? instance.GetInnerException() : nullptr)
		{
			// if (IS_DEBUG)
			// {
				std::cout << _ErrorMessage << std::endl;
			// }
		}

		[[nodiscard]] virtual String GetErrorMessage() const noexcept
		{
			return _ErrorMessage;
		}

		[[nodiscard]] String GetName() const {
			return _Name;
		}

		[[nodiscard]] int GetErrorCode() const noexcept {
			return _ErrorCode;
		}

		[[nodiscard]] std::unique_ptr<ExceptionBase> GetInnerException() const noexcept {
			return _InnerException ? _InnerException->Clone() : nullptr;
		}

		[[nodiscard]] virtual const char* what() const noexcept {
			return _ErrorMessage.c_str();
		}

		[[nodiscard]] String GetStackTrace(const String& tag = EMPTY_STRING, const String& header = DEFAULT_HEADER_PREFIX_STRING, const String& footer = DEFAULT_FOOTER_PREFIX_STRING) const {
			return (tag.empty() ? tag : header + " \"" + tag + "\"" + LINE_FEED_STRING) + _ErrorMessage + (_InnerException ? _InnerException->GetStackTrace(EMPTY_STRING) : EMPTY_STRING) + (tag.empty() ? tag : footer);
		}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override {
			return std::make_unique<ExceptionBase>(*this);
		}

	private:
		const String _Name;
		const String _ErrorMessage;
		const int _ErrorCode;
		const std::unique_ptr<ExceptionBase> _InnerException;
		String CreateExceptionMessage(const String& exceptionName, const String& function, const String& file, const int& line, const std::string& message, const int& errorCode)
		{
			return "Exception: \"" + exceptionName +
				"\" in the function: \""s + function +
				"\", line: \""s + std::to_string(line) +
				"\", file: \""s + file + "\". " +
				(message.empty() ? EMPTY_STRING : ("Message: \"" + message + "\". ")) +
				"ErrorCode: " + std::to_string(errorCode) +
				LINE_FEED_STRING + LINE_FEED_STRING;
		}
	};

	class  ParseException : public ExceptionBase
	{
	public:
		virtual ~ParseException() = default;
		ParseException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override {
			return std::make_unique<ParseException>(*this);
		}
	private:
	};

	class  CryptoException : public ExceptionBase
	{
	public:
		virtual ~CryptoException() = default;
		CryptoException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override {
			return std::make_unique<CryptoException>(*this);
		}
	private:
	};

	class  ArgumentException : public ExceptionBase
	{
	public:
		virtual ~ArgumentException() = default;
		ArgumentException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override {
			return std::make_unique<ArgumentException>(*this);
		}
	private:
	};

	class  NotImplementedException : public ExceptionBase
	{
	public:
		virtual ~NotImplementedException() = default;
		NotImplementedException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override {
			return std::make_unique<NotImplementedException>(*this);
		}
	private:
	};

	class  SerializeException : public ExceptionBase
	{
	public:
		virtual ~SerializeException() = default;
		SerializeException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override {
			return std::make_unique<SerializeException>(*this);
		}
	private:
	};

	class  DeserializeException : public ExceptionBase
	{
	public:
		virtual ~DeserializeException() = default;
		DeserializeException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override {
			return std::make_unique<DeserializeException>(*this);
		}
	private:
	};

	class  AbstractImplementationException : public ExceptionBase
	{
	public:
		virtual ~AbstractImplementationException() = default;
		AbstractImplementationException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override {
			return std::make_unique<AbstractImplementationException>(*this);
		}
	private:
	};

	class  ConvertException : public ExceptionBase
	{
	public:
		virtual ~ConvertException() = default;
		ConvertException(const String& function, const String& file, const int& line, const std::string& message = EMPTY_STRING, const int& code = 0, std::unique_ptr<ExceptionBase>&& innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override {
			return std::make_unique<ConvertException>(*this);
		}
	private:
	};
}
#endif //! INSANE_EXCEPTION_H
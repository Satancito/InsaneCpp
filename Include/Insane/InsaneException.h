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

	class ExceptionBase : public InsaneIO::Insane::Interfaces::IClone<ExceptionBase>, public std::exception
	{
	public:
		virtual ~ExceptionBase() = default;
		ExceptionBase(const String &name, const String &function, const String &file, const int &line, const String &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr)
			: _ErrorName(name),
			  _ErrorFunction(function),
			  _ErrorFile(file),
			  _ErrorLine(line),
			  _ErrorMessage(message),
			  _ErrorCode(code), 
			  _InnerException(innerException ? std::move(innerException) : nullptr)
		{
			//std::cout << GetStackTrace() << std::endl;
		}

		ExceptionBase(const ExceptionBase &instance)
			: _ErrorName(instance._ErrorName),
			  _ErrorFunction(instance._ErrorFunction),
			  _ErrorFile(instance._ErrorFile),
			  _ErrorLine(instance._ErrorLine),
			  _ErrorMessage(instance._ErrorMessage),
			  _ErrorCode(instance._ErrorCode), 
			  _InnerException(instance._InnerException ? instance.GetInnerException() : nullptr)
		{
			//std::cout << GetStackTrace() << std::endl;
		}

		[[nodiscard]] virtual String GetErrorMessage() const noexcept
		{
			return _ErrorMessage;
		}

		[[nodiscard]] String GetName() const
		{
			return _ErrorName;
		}

		[[nodiscard]] String GetFunction() const noexcept
		{
			return _ErrorFunction;
		}

		[[nodiscard]] String GetFile() const noexcept
		{
			return _ErrorFile;
		}

		[[nodiscard]] int GetLine() const noexcept
		{
			return _ErrorLine;
		}

		[[nodiscard]] int GetErrorCode() const noexcept
		{
			return _ErrorCode;
		}

		[[nodiscard]] std::unique_ptr<ExceptionBase> GetInnerException() const noexcept
		{
			return _InnerException ? _InnerException->Clone() : nullptr;
		}

		[[nodiscard]] virtual const char *what() const noexcept override
		{
			return _ErrorMessage.c_str();
		}

		[[nodiscard]] String GetStackTrace(const String &tag, const String &header = DEFAULT_HEADER_PREFIX_STRING, const String &footer = DEFAULT_FOOTER_PREFIX_STRING) const
		{
			return (tag.empty() ? tag : header + " \"" + tag + "\"" + LINE_FEED_STRING) + CreateExceptionMessage()  + (_InnerException ? LINE_FEED_STRING + LINE_FEED_STRING + _InnerException->GetStackTrace(EMPTY_STRING) : EMPTY_STRING) + (tag.empty() ? tag  :  LINE_FEED_STRING + footer);
		}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<ExceptionBase>(*this);
		}

	private:
		const String _ErrorName;
		const String _ErrorFunction;
		const String _ErrorFile;
		const int _ErrorLine;
		const String _ErrorMessage;
		const int _ErrorCode;
		const std::unique_ptr<ExceptionBase> _InnerException;
		String CreateExceptionMessage() const
		{
			return "Exception: \"" + _ErrorName +
				   "\" in the function: \""s + _ErrorFunction +
				   "\", line: \""s + std::to_string(_ErrorLine) +
				   "\", file: \""s + _ErrorFile + "\". " +
				   (_ErrorMessage.empty() ? EMPTY_STRING : ("Message: \"" + _ErrorMessage + "\". ")) +
				   "ErrorCode: " + std::to_string(_ErrorCode);
		}
	};

	class ParseException : public ExceptionBase
	{
	public:
		virtual ~ParseException() = default;
		ParseException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<ParseException>(*this);
		}

	private:
	};

	class CryptoException : public ExceptionBase
	{
	public:
		virtual ~CryptoException() = default;
		CryptoException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<CryptoException>(*this);
		}

	private:
	};

	class ArgumentException : public ExceptionBase
	{
	public:
		virtual ~ArgumentException() = default;
		ArgumentException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<ArgumentException>(*this);
		}

	private:
	};

	class NotImplementedException : public ExceptionBase
	{
	public:
		virtual ~NotImplementedException() = default;
		NotImplementedException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<NotImplementedException>(*this);
		}

	private:
	};

	class SerializeException : public ExceptionBase
	{
	public:
		virtual ~SerializeException() = default;
		SerializeException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<SerializeException>(*this);
		}

	private:
	};

	class DeserializeException : public ExceptionBase
	{
	public:
		virtual ~DeserializeException() = default;
		DeserializeException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<DeserializeException>(*this);
		}

	private:
	};

	class AbstractImplementationException : public ExceptionBase
	{
	public:
		virtual ~AbstractImplementationException() = default;
		AbstractImplementationException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<AbstractImplementationException>(*this);
		}

	private:
	};

	class ConvertException : public ExceptionBase
	{
	public:
		virtual ~ConvertException() = default;
		ConvertException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException)) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<ConvertException>(*this);
		}

	private:
	};
}
#endif //! INSANE_EXCEPTION_H
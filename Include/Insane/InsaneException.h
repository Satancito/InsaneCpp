#pragma once
#ifndef INSANE_EXCEPTION_H
#define INSANE_EXCEPTION_H

#include <Insane/Insane.h>
#include <Insane/InsanePreprocessor.h>
#include <memory>
#include <exception>
#include <string>
#define USING_NS_INSANE_EXCEPTION using namespace InsaneIO::Insane::Exception

#define DEFAULT_HEADER_PREFIX_STRING ("◢■■■■■■■■■■■■■■■■■■■■◣"s)
#define DEFAULT_FOOTER_PREFIX_STRING ("◥■■■■■■■■■■■■■■■■■■■■◤"s)

#define INSANE_DEBUG_EXCEPTION(tag, insaneExClass, message, code, innerEx, debugType)                                 \
	auto exToThrow = insaneExClass(INSANE_FUNCTION_SIGNATURE, __FILE__, __LINE__, message, code, innerEx, debugType); \
	exToThrow.Debug(tag);
#define INSANE_THROW_EXCEPTION(tag, insaneExClass, message, code, innerEx, debugType) INSANE_DEBUG_EXCEPTION(tag, insaneExClass, message, code, innerEx, debugType) throw exToThrow;

#define __INSANE_THROW_EXCEPTION(insaneExClass, message, code, innerEx, debugType) INSANE_THROW_EXCEPTION(INSANE_STRING, insaneExClass, message, code, innerEx, debugType)
#define __INSANE_DEBUG_EXCEPTION(insaneExClass, message, code, innerEx, debugType) INSANE_DEBUG_EXCEPTION(INSANE_STRING, insaneExClass, message, code, innerEx, debugType)
#define __INSANE_THROW_DEFAULT_EXCEPTION(insaneExClass, debugType) INSANE_THROW_EXCEPTION(INSANE_STRING, insaneExClass, "Unknown error.", 0, nullptr, debugType)
#define __INSANE_DEBUG_DEFAULT_EXCEPTION(insaneExClass, debugType) INSANE_DEBUG_EXCEPTION(INSANE_STRING, insaneExClass, "Unknown error.", 0, nullptr, debugType)
namespace InsaneIO::Insane::Exception
{

	enum class DebugType
	{
		Trace,		 // Neutral
		Debug,		 // Green
		Information, // Blue
		Error,		 // Red
		Critical,	 // DarkRed + Bold
		Warning		 // Yellow
	};

	class DebugTypeExtensions
	{
	public:
		[[nodiscard]] static String ToString(const DebugType &value) noexcept
		{
			switch (value)
			{
			case DebugType::Trace:
				return "Trace";
			case DebugType::Debug:
				return "Debug";
			case DebugType::Information:
				return "Information";
			case DebugType::Error:
				return "Error";
			case DebugType::Critical:
				return "Critical";
			case DebugType::Warning:
				return "Warning";
			}
		}
	};

	class ExceptionBase : public InsaneIO::Insane::Interfaces::IClone<ExceptionBase>,
						  public std::exception
	{
	public:
		virtual ~ExceptionBase() = default;
		ExceptionBase(const String &name, const String &function, const String &file, const int &line, const String &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr, DebugType debugType = DebugType::Trace)
			: _ErrorName(name),
			  _ErrorFunction(function),
			  _ErrorFile(file),
			  _ErrorLine(line),
			  _ErrorMessage(message),
			  _ErrorCode(code),
			  _InnerException(innerException ? std::move(innerException) : nullptr),
			  _DebugType(debugType)
		{
		}

		ExceptionBase(const ExceptionBase &instance)
			: _ErrorName(instance._ErrorName),
			  _ErrorFunction(instance._ErrorFunction),
			  _ErrorFile(instance._ErrorFile),
			  _ErrorLine(instance._ErrorLine),
			  _ErrorMessage(instance._ErrorMessage),
			  _ErrorCode(instance._ErrorCode),
			  _InnerException(instance._InnerException ? instance.GetInnerException() : nullptr),
			  _DebugType(instance.GeDebugType())
		{
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

		[[nodiscard]] DebugType GeDebugType() const noexcept
		{
			return _DebugType;
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
			return (tag.empty() ? tag : header + LINE_FEED_STRING + "▶ \"" + tag + "\"" + LINE_FEED_STRING + LINE_FEED_STRING) + CreateExceptionMessage() + (_InnerException ? LINE_FEED_STRING + LINE_FEED_STRING + _InnerException->GetStackTrace(EMPTY_STRING) : EMPTY_STRING) + (tag.empty() ? tag : LINE_FEED_STRING + footer);
		}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<ExceptionBase>(*this);
		}

		void Debug(const String &tag)
		{
			if (DebugExtensions::IsDebug())
			{
				std::cout << GetStackTrace(tag) << std::endl;
			}
		}

	private:
		const String _ErrorName;
		const String _ErrorFunction;
		const String _ErrorFile;
		const int _ErrorLine;
		const String _ErrorMessage;
		const int _ErrorCode;
		const std::unique_ptr<ExceptionBase> _InnerException;
		const DebugType _DebugType;
		String CreateExceptionMessage() const
		{
			return "Exception: \"" + _ErrorName + "\"" +
				   " in the function: \""s + _ErrorFunction + "\", "s +
				   " line: \""s + std::to_string(_ErrorLine) + "\", "s +
				   " file: \""s + _ErrorFile + "\".\n"s +
				   "Message: \""s + _ErrorMessage + "\".\n" +
				   "ErrorCode: \""s + std::to_string(_ErrorCode) + "\".\n" +
				   "DebugType: \"" + DebugTypeExtensions::ToString(_DebugType) + "\".";
		}
	};

	class ParseException : public ExceptionBase
	{
	public:
		virtual ~ParseException() = default;
		ParseException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr, DebugType debugType = DebugType::Trace)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException), debugType) {}

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
		CryptoException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr, DebugType debugType = DebugType::Trace)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException), debugType) {}

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
		ArgumentException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr, DebugType debugType = DebugType::Trace)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException), debugType) {}

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
		NotImplementedException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr, DebugType debugType = DebugType::Trace)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException), debugType) {}

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
		SerializeException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr, DebugType debugType = DebugType::Trace)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException), debugType) {}

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
		DeserializeException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr, DebugType debugType = DebugType::Trace)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException), debugType) {}

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
		AbstractImplementationException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr, DebugType debugType = DebugType::Trace)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException), debugType) {}

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
		ConvertException(const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr, DebugType debugType = DebugType::Trace)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException), debugType) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<ConvertException>(*this);
		}

	private:
	};

	class JsException  : public ExceptionBase
	{
	public:
		virtual ~JsException ()  = default;
		JsException (const String &function, const String &file, const int &line, const std::string &message = EMPTY_STRING, const int &code = 0, std::unique_ptr<ExceptionBase> &&innerException = nullptr, DebugType debugType = DebugType::Trace)
			: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException), debugType) {}

		[[nodiscard]] virtual std::unique_ptr<ExceptionBase> Clone() const override
		{
			return std::make_unique<JsException >(*this);
		}

	private:
	};
}
#endif //! INSANE_EXCEPTION_H
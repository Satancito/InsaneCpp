#include <Insane/InsaneException.h>
#include <utility>


static inline String InternalCreateExceptionMessage(const String& exceptionName, const String& function, const String& file, const int& line, const std::string& message, const int & errorCode)
{
	return "Exception: \"" + exceptionName +
		"\" in the function: \""s + function +
		"\", line: \""s + std::to_string(line) +
		"\", file: \""s + file + "\". " +
		(message.empty() ? EMPTY_STRING : ("Message: \"" + message + "\". ")) + 
		"ErrorCode: " + std::to_string(errorCode) +
		LINE_FEED_STRING + LINE_FEED_STRING;
}

InsaneIO::Insane::Exception::ExceptionBase::ExceptionBase(const String& name, const String& function, const String& file, const int& line, const String& message, const int& code, std::unique_ptr<ExceptionBase>&& innerException)
	: _Name(name), _ErrorMessage(InternalCreateExceptionMessage(name, function, file, line, message, code)), _ErrorCode(code), _InnerException(innerException ? std::move(innerException) : nullptr)
{
	if (IS_DEBUG)
	{
		std::cout << _ErrorMessage << std::endl;
	}
}

InsaneIO::Insane::Exception::ExceptionBase::ExceptionBase(const ExceptionBase& instance)
	:
	_Name(instance._Name),
	_ErrorMessage(instance._ErrorMessage),
	_ErrorCode(instance._ErrorCode),
	_InnerException(instance._InnerException ? instance._InnerException->Clone() : nullptr)
{
	if (IS_DEBUG)
	{
		std::cout << _ErrorMessage << std::endl;
	}
}

String InsaneIO::Insane::Exception::ExceptionBase::GetErrorMessage() const noexcept
{
	return _ErrorMessage;
}

String InsaneIO::Insane::Exception::ExceptionBase::GetName() const
{
	return _Name;
}

int InsaneIO::Insane::Exception::ExceptionBase::GetErrorCode() const noexcept
{
	return _ErrorCode;
}

std::unique_ptr<InsaneIO::Insane::Exception::ExceptionBase> InsaneIO::Insane::Exception::ExceptionBase::GetInnerException() const noexcept
{
	return _InnerException->Clone();
}

const char* InsaneIO::Insane::Exception::ExceptionBase::what() const noexcept
{
	return _ErrorMessage.c_str();
}

String InsaneIO::Insane::Exception::ExceptionBase::GetStackTrace(const String& tag, const String& headerPreffix, const String& footer) const const
{
	return (tag.empty() ? tag : headerPreffix + " \"" + tag + "\"" + LINE_FEED_STRING) + _ErrorMessage + (_InnerException ? _InnerException->GetStackTrace(EMPTY_STRING) : EMPTY_STRING) + (tag.empty() ? tag : footer );
}

InsaneIO::Insane::Exception::ParseException::ParseException(const String& function, const String& file, const int& line, const std::string& message, const int& code, std::unique_ptr<ExceptionBase>&& innerException)
	: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException))
{

}

std::unique_ptr<InsaneIO::Insane::Exception::ExceptionBase> InsaneIO::Insane::Exception::ParseException::Clone() const
{
	return std::make_unique<ParseException>(*this);
}

InsaneIO::Insane::Exception::CryptoException::CryptoException(const String& function, const String& file, const int& line, const std::string& message, const int& code, std::unique_ptr<ExceptionBase>&& innerException)
	: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException))
{

}

std::unique_ptr<InsaneIO::Insane::Exception::ExceptionBase> InsaneIO::Insane::Exception::CryptoException::Clone() const
{
	return std::make_unique<CryptoException>(*this);
}

InsaneIO::Insane::Exception::ArgumentException::ArgumentException(const String& function, const String& file, const int& line, const std::string& message, const int& code, std::unique_ptr<ExceptionBase>&& innerException)
	: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException))
{

}

std::unique_ptr<InsaneIO::Insane::Exception::ExceptionBase> InsaneIO::Insane::Exception::ArgumentException::Clone() const
{
	return std::make_unique<ArgumentException>(*this);
}

InsaneIO::Insane::Exception::NotImplementedException::NotImplementedException(const String& function, const String& file, const int& line, const std::string& message, const int& code, std::unique_ptr<ExceptionBase>&& innerException)
	: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException))
{

}

std::unique_ptr<InsaneIO::Insane::Exception::ExceptionBase> InsaneIO::Insane::Exception::NotImplementedException::Clone() const
{
	return std::make_unique<NotImplementedException>(*this);
}

InsaneIO::Insane::Exception::SerializeException::SerializeException(const String& function, const String& file, const int& line, const std::string& message, const int& code, std::unique_ptr<ExceptionBase>&& innerException)
	: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException))
{

}

std::unique_ptr<InsaneIO::Insane::Exception::ExceptionBase> InsaneIO::Insane::Exception::SerializeException::Clone() const
{
	return std::make_unique<SerializeException>(*this);
}

InsaneIO::Insane::Exception::DeserializeException::DeserializeException(const String& function, const String& file, const int& line, const std::string& message, const int& code, std::unique_ptr<ExceptionBase>&& innerException)
	: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException))
{

}

std::unique_ptr<InsaneIO::Insane::Exception::ExceptionBase> InsaneIO::Insane::Exception::DeserializeException::Clone() const
{
	return std::make_unique<DeserializeException>(*this);
}

InsaneIO::Insane::Exception::AbstractImplementationException::AbstractImplementationException(const String& function, const String& file, const int& line, const std::string& message, const int& code, std::unique_ptr<ExceptionBase>&& innerException)
	: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException))
{

}

std::unique_ptr<InsaneIO::Insane::Exception::ExceptionBase> InsaneIO::Insane::Exception::AbstractImplementationException::Clone() const
{
	return std::make_unique<AbstractImplementationException>(*this);
}

InsaneIO::Insane::Exception::ConvertException::ConvertException(const String& function, const String& file, const int& line, const std::string& message, const int& code, std::unique_ptr<ExceptionBase>&& innerException)
	: ExceptionBase(__FUNCTION__, function, file, line, message, code, std::move(innerException))
{

}

std::unique_ptr<InsaneIO::Insane::Exception::ExceptionBase> InsaneIO::Insane::Exception::ConvertException::Clone() const
{
	return std::make_unique<ConvertException>(*this);
}

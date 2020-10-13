#include <Insane/InsaneException.h>

Insane::Exception::ExceptionBase::ExceptionBase(const String& _message, const int& _code ) :message(_message), code(_code)
{
}

std::string Insane::Exception::ExceptionBase::Message() const
{
	return this->message;
}

int Insane::Exception::ExceptionBase::Code() const
{
	return this->code;
}

Insane::Exception::ParseException::ParseException(const String& _message, const int& _code) : ExceptionBase(_message, _code)
{
}

Insane::Exception::CryptoException::CryptoException(const String& _message, const int& _code) : ExceptionBase(_message, _code)
{
}


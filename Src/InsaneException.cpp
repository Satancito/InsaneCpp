#include <Insane/InsaneException.h>

// ███ ExceptionBase ███

Insane::Exception::ExceptionBase::ExceptionBase(const String& _message, const int& _code ) :message(_message), code(_code)
{

}

String Insane::Exception::ExceptionBase::Message() const
{
	return this->message;
}

int Insane::Exception::ExceptionBase::Code() const
{
	return this->code;
}

// ███ ParseException ███

Insane::Exception::ParseException::ParseException(const String& _message, const int& _code) : ExceptionBase(_message, _code)
{
}

// ███ CryptoException ███

Insane::Exception::CryptoException::CryptoException(const String& _message, const int& _code) : ExceptionBase(_message, _code)
{
}

// ███ ArgumentException ███

Insane::Exception::ArgumentException::ArgumentException(const String& _message, const int& _code) : ExceptionBase(_message, _code)
{
}

// ███ NotImplementedException ███

Insane::Exception::NotImplementedException::NotImplementedException(const String& _message, const int& _code) : ExceptionBase(_message, _code)
{
}

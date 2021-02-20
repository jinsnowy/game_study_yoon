#include "UserException.h"
#include <sstream>

UserException::UserException(int codeLine, const char* fileName) noexcept
	:
	erCodeLine(codeLine),
	erFileName(fileName)
{
}

const char* UserException::what() const noexcept
{
	ostringstream oss;
	oss << "[Type] " << GetType() << endl
		<< GetErrorMessage() << endl;

	m_whatBuffer = oss.str();
	return m_whatBuffer.c_str();
}

const char* UserException::GetType() const noexcept
{
	return "My Exception";
}

string UserException::GetErrorMessage() const noexcept
{
	ostringstream oss;
	oss << "[File] " << erFileName << endl
		<< "[Line] " << erCodeLine;
	return oss.str();
}

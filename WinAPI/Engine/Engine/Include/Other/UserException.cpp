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

	mWhatBuffer = oss.str();
	return mWhatBuffer.c_str();
}

const char* UserException::GetType() const noexcept
{
	return "User Exception";
}

string UserException::GetErrorMessage() const noexcept
{
	ostringstream oss;
	oss << "[File] " << erFileName << endl
		<< "[Line] " << erCodeLine;
	return oss.str();
}

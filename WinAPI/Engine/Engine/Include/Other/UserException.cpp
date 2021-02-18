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
	std::ostringstream oss;
	oss << "[Type] " << GetType() << std::endl
		<< GetErrorMessage() << std::endl;

	mWhatBuffer = oss.str();
	return mWhatBuffer.c_str();
}

const char* UserException::GetType() const noexcept
{
	return "User Exception";
}

std::string UserException::GetErrorMessage() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << erFileName << std::endl
		<< "[Line] " << erCodeLine;
	return oss.str();
}

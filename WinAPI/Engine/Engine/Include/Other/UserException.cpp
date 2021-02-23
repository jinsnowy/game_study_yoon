#include "UserException.h"
#include <sstream>

UserException::UserException(int codeLine, const char* fileName) noexcept
	:
	erCodeLine(codeLine),
	erFileName(fileName)
{
}

const wchar_t* UserException::What() const noexcept
{
	wstringstream oss;
	oss << "[Type] " << GetType() << endl
		<< GetErrorMessage() << endl;

	m_whatBuffer = oss.str();
	return m_whatBuffer.c_str();
}

const wchar_t* UserException::GetType() const noexcept
{
	return L"My Exception";
}

wstring UserException::GetErrorMessage() const noexcept
{
	wstringstream oss;
	wstring fileName(erFileName.begin(), erFileName.end());
	oss << "[File] " << fileName << endl
		<< "[Line] " << erCodeLine;
	return oss.str();
}

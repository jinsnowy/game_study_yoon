#pragma once
#include <string>
#include <sstream>
#include <memory>
#include <cstring>
#include <windows.h>

namespace util
{
	static char _char_buffer[MAX_PATH] = {0};
	static wchar_t _wchar_buffer[MAX_PATH] = {0};
	const wchar_t* GetWChar(const char* c);
	const char* GetChar(const wchar_t* c);
}

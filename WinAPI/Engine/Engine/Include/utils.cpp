#include "utils.h"


const wchar_t* util::GetWChar(const char* c)
{
	int cSize = strlen(c) + 1;
	memset(_wchar_buffer, 0, sizeof(_wchar_buffer));
	mbstowcs(_wchar_buffer, c, cSize);
	return _wchar_buffer;
}

const char* util::GetChar(const wchar_t* c)
{
	int cSize = lstrlen(c) + 1;
	memset(_char_buffer, 0, sizeof(_char_buffer));
	WideCharToMultiByte(CP_ACP, 0, c, -1, _char_buffer, lstrlen(c), 0, 0);
	return _char_buffer;
}

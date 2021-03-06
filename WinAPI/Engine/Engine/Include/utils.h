#pragma once
#include <string>
#include <string.h>
#include <sstream>
#include <memory>
#include <windows.h>


namespace util
{
	static char _char_buffer[MAX_PATH] = {0};
	static wchar_t _wchar_buffer[MAX_PATH] = {0};
	const wchar_t* GetWChar(const char* c);
	const char* GetChar(const wchar_t* c);
	inline RECT MakeRect(int left, int top, int w, int h)
	{
		RECT rc;
		rc.left = left;
		rc.top = top;
		rc.right = rc.left + w;
		rc.bottom = rc.top + h;
		return rc;
	}
	void DrawRedRect(HDC hdc, RECT rc);
	void DrawRectWithColor(HDC hdc, RECT rc, COLORREF color);
	void DrawHDCWithColor(HDC hdc, int w, int h, COLORREF color);
	void DrawHDCWithColor(HDC hdc, int px, int py, int w, int h, COLORREF color);
	std::string ExtractKeyFromPathString(const char* str, int size);
}

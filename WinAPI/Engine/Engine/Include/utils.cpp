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

void util::DrawRedRect(HDC hdc, RECT rc)
{
	HPEN myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	HPEN OldPen = (HPEN)SelectObject(hdc, myPen);
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
	DeleteObject(SelectObject(hdc, OldPen));
	DeleteObject(SelectObject(hdc, OldBrush));
}

void util::DrawHDCWithColor(HDC hdc, int w, int h, COLORREF color)
{
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(color));

	Rectangle(hdc, -5, -5, w+5, h+5);

	DeleteObject(SelectObject(hdc, OldBrush));
}

void util::DrawHDCWithColor(HDC hdc, int px, int py, int w, int h, COLORREF color)
{
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(color));

	Rectangle(hdc, px-5, py-5, px + w + 5, py + h + 5);

	DeleteObject(SelectObject(hdc, OldBrush));
}

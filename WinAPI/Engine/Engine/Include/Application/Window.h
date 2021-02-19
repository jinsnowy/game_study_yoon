#pragma once
#include "../framework.h"
#include "../Other/UserException.h"

// error exception helper macro
#define WND_EXCEPT(hr) Window::WindowException(__LINE__, __FILE__, hr);
#define WND_LAST_EXCEPT() Window::WindowException(__LINE__, __FILE__, GetLastError());

class Window
{
	DECLARE_SINGLE(Window)
private:
	class WindowException : public UserException
	{
	public:
		WindowException(int codeLine, const char* fileName, HRESULT hr) noexcept;
		virtual const char* GetType() const noexcept override;
		virtual string GetErrorMessage() const noexcept override;
	private:
		HRESULT hr;
		static string TranslateErrorCode(HRESULT hr) noexcept;
	};
private:
	static constexpr const char* wndClassName = "myEngine";
private:
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HDC m_hDC;
	RESOLUTION m_RS;
public:
	BOOL Init(int width, int height, const char* name);
	HDC GetWndDC() noexcept	{ return m_hDC; }
	RESOLUTION GetResolution() noexcept { return m_RS; }
	static const char* GetName() { return wndClassName; }
private:
	friend class WindowBase;
	void MyRegisterClass();
	void SetTitle(const string& title);

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};


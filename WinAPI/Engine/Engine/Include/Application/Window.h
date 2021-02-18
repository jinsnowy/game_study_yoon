#pragma once
#include "../framework.h"
#include "../Other/UserException.h"
#include "WindowBase.h"

// error exception helper macro
#define WND_EXCEPT(hr) Window::WindowException(__LINE__, __FILE__, hr);
#define WND_LAST_EXCEPT() Window::WindowException(__LINE__, __FILE__, GetLastError());

class Window : public WindowBase<Window>
{
private:
	class WindowException : public UserException
	{
	public:
		WindowException(int codeLine, const char* fileName, HRESULT hr) noexcept;
		virtual const char* GetType() const noexcept override;
		virtual std::string GetErrorMessage() const noexcept override;
	private:
		HRESULT hr;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
private:
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HDC m_hDC;
	RESOLUTION m_RS;
public:
	BOOL Init(int width, int height, const char* name);
	HDC GetWndDC() { return m_hDC; }
	RESOLUTION GetResolution() noexcept { return m_RS; }
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	friend class WindowBase;
	Window();
	~Window();
	void MyRegisterClass();
	void SetTitle(const std::string& title);

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};


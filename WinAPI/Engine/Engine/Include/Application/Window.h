#pragma once

#include "../framework.h"
#include "../Other/UserException.h"

// error exception helper macro
#define WND_EXCEPT(hr) Window::WindowException(__LINE__, __FILE__, hr);
#define WND_LAST_EXCEPT() Window::WindowException(__LINE__, __FILE__, GetLastError());

class Window
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
	class WindowClass {
	private:
		static WindowClass wndClass;
		static constexpr const char* wndClassName = "myEngine";
		HINSTANCE hInst;
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
	};
private:
	RESOLUTION m_RS = { 600, 800 };
	HWND m_hWnd;
	HDC m_hDC;
public:
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	friend class App;
	~Window();
	HDC GetWndDC() { return m_hDC; }
	void SetTitle(const std::string& title);
	Window(int width, int height, const char* name);
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};


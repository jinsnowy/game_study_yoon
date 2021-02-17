#pragma once

#include "framework.h"
#include "UserException.h"
#include "Graphics.h"

// error exception helper macro
#define WND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr);
#define WND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError());

class Window
{
private:
	// Window 초기화시 발생하는 예외 처리
	class Exception : public UserException
	{
	public:
		Exception(int codeLine, const char* fileName, HRESULT hr) noexcept;
		virtual const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		virtual std::string GetErrorMessage() const noexcept override;
	private:
		HRESULT hr;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};

private:
	// 초기화에 필요한 정보(mWndClassName)과 HINSTANCE를 숨긴다.
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
	int mWidth = 600;
	int mHeight = 800;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
public:
	~Window();
	Window(int width, int height, const char* name);
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	Graphics& Gfx();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};


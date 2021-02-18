#pragma once
#include "../framework.h"
#include "../Other/UserException.h"
#include "Window.h"

#define APP_EXCEPT(MSG) App::AppException(__LINE__, __FILE__, MSG);

class App
{
public:
	class AppException : public UserException
	{
	private:
		std::string message;
	public:
		AppException(int codeLine, const char* fileName, std::string message);
		virtual const char* GetType() const noexcept;
		virtual std::string GetErrorMessage() const noexcept;
	};
public:
	App();
	~App();
	// master frame / message loop
	int Go();
private:
	void Process();
	void Input(float dt);
	void Update(float dt);
	void LateUpdate(float dt);
	void Collision(float dt);
	void Draw(float dt);
private:
	Window wnd;
};


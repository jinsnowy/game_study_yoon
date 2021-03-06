#pragma once
#include "../framework.h"
#include "../Other/UserException.h"
#define APP_EXCEPT(MSG) App::AppException(__LINE__, __FILE__, MSG);

class App
{
public:
	class AppException : public UserException
	{
	private:
		wstring message;
	public:
		AppException(int codeLine, const char* fileName, const wchar_t* message);
		virtual const wchar_t* GetType() const noexcept;
		virtual wstring GetErrorMessage() const noexcept;
	};
public:
	App();
	~App();
	void Init();
	int Go();
private:
	void Process();
	void Input(float dt);
	int Update(float dt);
	int LateUpdate(float dt);
	void Collision(float dt);
	void Draw(float dt);

private:
	static constexpr float m_fSceneDelay = 1.0f;
	static constexpr float m_fSceneDrawPeriod = 0.04f;
	float m_fDelay = 0.f;
	void DrawSceneChange();
};


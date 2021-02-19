#include "App.h"
#include "Window.h"
#include "../Core/FrameTimer.h"
#include "../Scene/SceneManager.h"
#include "../Core/PathManager.h"
#include "../Resources/ResourceManager.h"

App::App()
{
	if (!WINDOW.Init(1200, 900, "My First Game"))
	{
		throw APP_EXCEPT("Window init failed.\n");
	}

	if (!TIMER.Init())
	{
		throw APP_EXCEPT("Timer init failed.\n");
	}

	if (!PATH_MANAGER.Init())
	{
		throw APP_EXCEPT("Path Manager init failed.\n");
	}

	if (!SCENE_MANAGER.Init())
	{
		throw APP_EXCEPT("SceneManager init failed.\n");
	}

	if (!RESOURCE_MANAGER.Init(WINDOW.m_hInst, WINDOW.m_hDC))
	{
		throw APP_EXCEPT("ResourceManager init failed.\n");
	}

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(237);
}

App::~App()
{
}

int App::Go()
{
	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				return -1;
			}
		}
		else {
			// Game Frame goes
			Process();
			// optional to deal with some functions after frame update
			/*
			  msg.message = WM_RENDER_RESET;
			  DispatchMessage(&msg);
			*/
		}
	}

	return (int)msg.wParam;
}

void App::Process()
{
	const float dt = TIMER.Tick();

	Input(dt);
	Update(dt);
	LateUpdate(dt);
	Collision(dt);
	Draw(dt);
}

void App::Input(float dt)
{
	SCENE_MANAGER.Input(dt);
}

void App::Update(float dt)
{
	SCENE_MANAGER.Update(dt);
}

void App::LateUpdate(float dt)
{
	SCENE_MANAGER.LateUpdate(dt);
}

void App::Collision(float dt)
{
	SCENE_MANAGER.Collision(dt);
}

void App::Draw(float dt)
{
	SCENE_MANAGER.Draw(WINDOW.m_hDC, dt);
}

// Error handling
App::AppException::AppException(int codeLine, const char* fileName, const char* message)
	: UserException(codeLine, fileName), message(message)
{
}

const char* App::AppException::GetType() const noexcept
{
	return "App Exception";
}

string  App::AppException::GetErrorMessage() const noexcept
{
	return App::AppException::UserException::GetErrorMessage() + "\n" + message;
}

#include "App.h"
#include "Window.h"
#include "../Core/FrameTimer.h"
#include "../Core/PathManager.h"
#include "../Core/Camera.h"
#include "../Core/Input.h"
#include "../Collider/CollisionManager.h"
#include "../Scene/SceneManager.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"


App::App()
{
	if (!WINDOW.Init(1280, 720, "My First Game"))
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
	
	if (!RESOURCE_MANAGER.Init(WINDOW.m_hInst, WINDOW.m_hDC))
	{
		throw APP_EXCEPT("ResourceManager init failed.\n");
	}

	if (!INPUT.Init(WINDOW.m_hWnd))
	{
		throw APP_EXCEPT("Input init failed.\n");
	}

	if (!COLLISION_MANAGER.Init())
	{
		throw APP_EXCEPT("Collision Manager init failed.\n");
	}

	// 씬 초기화 전에 카메라 초기화
	if (!CAMERA.Init(Pos(0.f, 0.f), GETRESOLUTION, RESOLUTION(1920, 1080)))
	{
		throw APP_EXCEPT("Camera init failed.\n");
	}

	if (!SCENE_MANAGER.Init())
	{
		throw APP_EXCEPT("SceneManager init failed.\n");
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
	CAMERA.Input(dt);
}

void App::Update(float dt)
{
	INPUT.Update(dt);
	SCENE_MANAGER.Update(dt);
	CAMERA.Update(dt);
}

void App::LateUpdate(float dt)
{
	SCENE_MANAGER.LateUpdate(dt);
}

void App::Collision(float dt)
{
	SCENE_MANAGER.Collision(dt);
	COLLISION_MANAGER.Collision(dt);
}

void App::Draw(float dt)
{
	// 더블 버퍼링
	Texture* pBackBuffer = RESOURCE_MANAGER.GetBackBuffer();

	Rectangle(pBackBuffer->GetDC(), 0, 0, 1280, 720);

	SCENE_MANAGER.Draw(pBackBuffer->GetDC(), dt);

	BitBlt(WINDOW.m_hDC, 0, 0, GETRESOLUTION.x, GETRESOLUTION.y, pBackBuffer->GetDC(), 0, 0, SRCCOPY);

	SAFE_RELEASE(pBackBuffer);
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

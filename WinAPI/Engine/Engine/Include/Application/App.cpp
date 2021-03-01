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
#include "../Sound/SoundManager.h"
#include "../Object/Mouse.h"


App::App()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// _CrtSetBreakAlloc(2952);
#ifdef _DEBUG
	// 콘솔창을 생성시켜준다
	AllocConsole();
#endif
}

App::~App()
{
#ifdef _DEBUG
	FreeConsole();
#endif

	SCENE_MANAGER->Release();
	CAMERA->Release();
	COLLISION_MANAGER->Release();
	INPUT->Release();
	RESOURCE_MANAGER->Release();
	SOUND_MANAGER->Release();
	PATH_MANAGER->Release();
	TIMER->Release();
	WINDOW->Release();
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
		}
	}

	return (int)msg.wParam;
}

void App::Init()
{
	if (!WINDOW->Init(1280, 720, L"My First Game"))
	{
		throw APP_EXCEPT(L"Window init failed.\n");
	}

	if (!TIMER->Init(WINDOW->m_hWnd))
	{
		throw APP_EXCEPT(L"Timer init failed.\n");
	}

	if (!PATH_MANAGER->Init())
	{
		throw APP_EXCEPT(L"Path Manager init failed.\n");
	}

	if (!RESOURCE_MANAGER->Init(WINDOW->m_hInst, WINDOW->m_hDC))
	{
		throw APP_EXCEPT(L"ResourceManager init failed.\n");
	}

	if (!SOUND_MANAGER->Init())
	{
		throw APP_EXCEPT(L"SoundManager init failed.\n");
	}

	if (!COLLISION_MANAGER->Init())
	{
		throw APP_EXCEPT(L"Collision Manager init failed.\n");
	}

	if (!INPUT->Init(WINDOWHANDLE))
	{
		throw APP_EXCEPT(L"Input init failed.\n");
	}

	// 씬 초기화 전에 카메라 초기화
	if (!CAMERA->Init(Pos(0.f, 0.f), GETRESOLUTION, RESOLUTION(1920, 1080)))
	{
		throw APP_EXCEPT(L"Camera init failed.\n");
	}

	if (!SCENE_MANAGER->Init())
	{
		throw APP_EXCEPT(L"SceneManager init failed.\n");
	}
}

void App::Process()
{
	const float dt = TIMER->Tick();

	Input(dt);

	if (Update(dt) == SC_CHANGE)
	{
		return;
	}
		
	if (LateUpdate(dt) == SC_CHANGE)
	{
		return;
	}
		

	Collision(dt);
	Draw(dt);
}

void App::Input(float dt)
{
	INPUT->Update(dt);
	SCENE_MANAGER->Input(dt);
	CAMERA->Input(dt);
}

int App::Update(float dt)
{
	SCENE_CHANGE sc;
	sc = SCENE_MANAGER->Update(dt);
	CAMERA->Update(dt);
	SOUND_MANAGER->Update(dt);
	return sc;
}

int App::LateUpdate(float dt)
{
	SCENE_CHANGE sc;
	sc = SCENE_MANAGER->LateUpdate(dt);
	return sc;
}

void App::Collision(float dt)
{
	SCENE_MANAGER->Collision(dt);
	COLLISION_MANAGER->Collision(dt);
}

void App::Draw(float dt)
{
	// 더블 버퍼링
	Texture* pBackBuffer = RESOURCE_MANAGER->GetBackBuffer();

	SCENE_MANAGER->Draw(pBackBuffer->GetDC(), dt);

	BitBlt(WINDOW->m_hDC, 0, 0, GETRESOLUTION.x, GETRESOLUTION.y, pBackBuffer->GetDC(), 0, 0, SRCCOPY);

	SAFE_RELEASE(pBackBuffer);

	Mouse* pMouse = INPUT->GetMouse();

	pMouse->Draw(WINDOW->m_hDC, dt);
}

// Error handling
App::AppException::AppException(int codeLine, const char* fileName, const wchar_t* message)
	: UserException(codeLine, fileName), message(message)
{
}

const wchar_t* App::AppException::GetType() const noexcept
{
	return L"App Exception";
}

wstring App::AppException::GetErrorMessage() const noexcept
{
	return App::AppException::UserException::GetErrorMessage() + L"\n" + message;
}

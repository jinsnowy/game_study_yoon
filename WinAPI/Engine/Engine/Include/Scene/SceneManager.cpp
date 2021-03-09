#include "SceneManager.h"
#include "StartScene.h"
#include "MapEditScene.h"
#include "InHouseScene.h"
#include "../Object/Object.h"
#include "../Application/Window.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "../Collider/CollisionManager.h"
#include "../Core/FrameTimer.h"

DEFINITION_SINGLE(SceneManager)

SceneManager::SceneManager()
	:
	m_pScene(nullptr)
{
	m_vecScene.resize(SC_END, nullptr);
}

SceneManager::~SceneManager()
{
	for (Scene* sc : m_vecScene)
	{
		SAFE_DELETE(sc);
	}
	m_vecScene.clear();
}

bool SceneManager::Init()
{
	m_pScene = CreateScene<StartScene>(SCENE_CREATE::SC_START);
	CreateScene<MapEditScene>(SCENE_CREATE::SC_MAPEDIT);
	// CreateScene<InHouseScene>(SCENE_CREATE::SC_INHOUSE);
	return true;
}

void SceneManager::Input(float dt)
{
	m_pScene->Input(dt);
	ChangeShowMode();
}

int SceneManager::Update(float dt)
{
	m_pScene->Update(dt);
	return m_iSignal;
}

int SceneManager::LateUpdate(float dt)
{
	m_pScene->LateUpdate(dt);
	return m_iSignal;
}

void SceneManager::Collision(float dt)
{
	m_pScene->Collision(dt);
}

void SceneManager::Draw(HDC hdc, float dt)
{
	m_pScene->Draw(hdc, dt);
}

void SceneManager::ChangeScene()
{
	FadeOut();

	if (m_vecScene[m_eNext] == nullptr)
	{
		switch (m_eNext)
		{
		case SCENE_CREATE::SC_INHOUSE:
			CreateScene<InHouseScene>(m_eNext);
			break;
		}
	}

	m_pScene = m_vecScene[m_eNext];
	COLLISION_MANAGER->Clear();
	m_iSignal = 0;
	m_eNext = SC_END;
}

void SceneManager::ChangeShowMode()
{
	if (GetAsyncKeyState(VK_F1) & 0x8000)
	{
		m_eCurShowMode = SHOW_ALL;
	}
	if (GetAsyncKeyState(VK_F2) & 0x8000)
	{
		m_eCurShowMode = SHOW_GROUND;
	}
	if (GetAsyncKeyState(VK_F3) & 0x8000)
	{
		m_eCurShowMode = SHOW_OBJECT;
	}
	if (GetAsyncKeyState(VK_F4) & 0x8000)
	{
		m_eCurShowMode = SHOW_STATIC;
	}
}

void SceneManager::FadeOut()
{
	const int RSW = GETRESOLUTION.x;
	const int RSH = GETRESOLUTION.y;

	Texture* pBackBuffer = RESOURCE_MANAGER->GetBackBuffer();
	Texture* pEmptyTex = Texture::CreateEmptyTexture(WINDOW->GetWndDC(), RSW, RSH);

	m_fDelay = 0.f;
	RESOURCE_MANAGER->SetAlphaChannel(0);
	float th = m_fSceneDrawPeriod;
	while (m_fDelay < m_fSceneDelay)
	{
		const float dt = TIMER->Tick();
		// 장면 전환 효과
		m_fDelay += dt;
		if (m_fDelay > th)
		{
			th += m_fSceneDrawPeriod;
			int alpha = int(255.f * (m_fDelay / m_fSceneDelay));
			RESOURCE_MANAGER->SetAlphaChannel(alpha);
			AlphaBlend(WINDOW->GetWndDC(), 0, 0, RSW, RSH,
				pEmptyTex->GetDC(), 0, 0, RSW, RSH, RESOURCE_MANAGER->GetBlendFunc());
		}
	}

	SAFE_RELEASE(pBackBuffer);
	SAFE_RELEASE(pEmptyTex);
}

void SceneManager::SignalizeSceneChange(SCENE_CREATE next)
{
	m_iSignal = m_iChangeSignal;
	m_eNext = next;
}


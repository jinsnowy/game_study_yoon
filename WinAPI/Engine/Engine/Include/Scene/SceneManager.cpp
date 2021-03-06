#include "SceneManager.h"
#include "StartScene.h"
#include "MapEditScene.h"
#include "InHouseScene.h"
#include "FarmScene.h"
#include "../Core/FrameTimer.h"
#include "../Object/Object.h"
#include "../Application/Window.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "../Core/Input.h"
#include "../Collider/CollisionManager.h"
#include "../Object/StaticObj/UIGameManager.h"
#include "../Core/FrameTimer.h"
#include "../Sound/SoundManager.h"
#include "../Object/MoveObj/Player.h"

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
	SAFE_RELEASE(m_pPlayer);

	UI_MANAGER->Release();
}

bool SceneManager::Init()
{
	INPUT->AddKey("ShowOption", VK_F1);
	INPUT->AddKey("ShowGrid", VK_F2);
	INPUT->AddKey("ShowColl", VK_F3);
	INPUT->AddKey("TimeElapse", VK_CONTROL, 'T');
	// INPUT->AddKey("TimeReset", VK_CONTROL, 'R');
	SOUND_MANAGER->LoadSound("LongBGM", false, SD_BACKGROUND, "LongBGM.mp3");
	SOUND_MANAGER->LoadSound("BGM", true, SD_BACKGROUND, "BGM.mp3");

	// 모든 텍스쳐 초기화
	CreateScene<MapEditScene>(SC_MAPEDIT);
	SAFE_DELETE(m_vecScene[SC_MAPEDIT]);

	m_pScene = CreateScene<StartScene>(SC_START);

	m_pPlayer = new Player;
	m_pPlayer->SetTag("Player");
	m_pPlayer->Init();

	UI_MANAGER->SetPlayer(m_pPlayer);

	m_tNextState.nextScene = SC_NONE;
	m_tNextState.nextBeacon = BC_NONE;
	m_tNextState.nextDir = RIGHT;

	if (!UI_MANAGER->Init())
	{
		throw EXCEPT(L"UI init failed.\n");
	}

	return true;
}

void SceneManager::Input(float dt)
{
	ChangeShowMode();
	if (KEYPRESS("TimeElapse"))
	{
		UI_MANAGER->IncreaseTime();
	}
	m_pScene->Input(dt);
	UI_MANAGER->Input(dt);

	//if (SOUND_MANAGER->IsEnd("StartLongBGM") && SOUND_MANAGER->IsEnd(SD_BACKGROUND))
	//{
	//	SOUND_MANAGER->PlaySound("BGM");
	//}
}

int SceneManager::Update(float dt)
{
	m_pScene->Update(dt);
	UI_MANAGER->Update(dt);
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

	if(m_pScene->GetSceneType() != SC_START && m_pScene->GetSceneType() != SC_MAPEDIT)
		UI_MANAGER->Draw(hdc, dt);
}

void SceneManager::ChangeScene()
{
	FadeOut();

	SCENE_CREATE cur = m_pScene->GetSceneType();
	switch (cur)
	{
	case SCENE_CREATE::SC_MAPEDIT:
		SAFE_DELETE(m_vecScene[cur]);
		break;
	}

	SCENE_CREATE nxt = m_tNextState.nextScene;
	if (!m_vecScene[nxt])
	{
		switch (nxt)
		{
		case SCENE_CREATE::SC_INHOUSE:
			CreateScene<InHouseScene>(nxt);
			UI_MANAGER->StartTick();
			break;
		case SCENE_CREATE::SC_MAPEDIT:
			CreateScene<MapEditScene>(nxt);
			break;
		case SCENE_CREATE::SC_FARM:
			CreateScene<FarmScene>(nxt);
			break;
		}
	}

	m_pScene = m_vecScene[nxt];
	
	GameScene* gameScene = dynamic_cast<GameScene*>(m_pScene);
	if (gameScene)
	{
		gameScene->SetUpScene(m_tNextState, m_pPlayer);
	}

	COLLISION_MANAGER->Clear();
	m_iSignal = 0;
	m_tNextState.nextBeacon = BC_NONE;
	m_tNextState.nextDir = LEFT;
	m_tNextState.nextScene = SC_NONE;
}

void SceneManager::ChangeShowMode()
{
	if (KEYDOWN("ShowOption"))
	{
		ToggleShowMode(SHOW_TILEOPTION);
	}
	if (KEYDOWN("ShowGrid"))
	{
		ToggleShowMode(SHOW_GRID);
	}
	if (KEYDOWN("ShowColl"))
	{
		ToggleShowMode(SHOW_COLL);
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

void SceneManager::SignalizeSceneChange(SceneState state)
{
	m_iSignal = m_iChangeSignal;
	m_tNextState = state;
}


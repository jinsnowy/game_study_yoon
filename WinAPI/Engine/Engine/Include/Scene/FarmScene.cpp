#include "FarmScene.h"
#include "../Object/MoveObj/Player.h"
#include "SceneManager.h"
#include"../Sound/SoundManager.h"
FarmScene::FarmScene()
{
}

FarmScene::~FarmScene()
{
}

bool FarmScene::Init()
{
	GameScene::SetUpScene(pGameDataFileName);

	SOUND_MANAGER->LoadSound("DoorOpen", false, SD_EFFECT, "DoorOpen.mp3");

	return true;
}

void FarmScene::Input(float dt)
{
	GameScene::Input(dt);
}

void FarmScene::Update(float dt)
{
	GameScene::Update(dt);
}

void FarmScene::LateUpdate(float dt)
{
	GameScene::LateUpdate(dt);
	Pos curPos = AccessPlayer()->GetCenterPos();
	TILE_OPTION option = GetTileOption(curPos);
	switch (option)
	{
	case TO_BEACON_1:
		SceneState state;
		state.nextDir = UP;
		state.nextBeacon = BC_ONE;
		state.nextScene = SC_INHOUSE;

		SOUND_MANAGER->PlaySound("DoorOpen");
		SCENE_MANAGER->SignalizeSceneChange(state);
		break;
	}
}

void FarmScene::Collision(float dt)
{
	GameScene::Collision(dt);
}

void FarmScene::Draw(HDC hdc, float dt)
{
	GameScene::Draw(hdc, dt);
}

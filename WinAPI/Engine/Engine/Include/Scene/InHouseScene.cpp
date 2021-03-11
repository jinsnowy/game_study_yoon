#include "InHouseScene.h"
#include "../Object/MoveObj/Player.h"
#include "SceneManager.h"
#include "../Sound/SoundManager.h"

InHouseScene::InHouseScene()
{
}

InHouseScene::~InHouseScene()
{
}

bool InHouseScene::Init()
{
	GameScene::SetUpScene(pGameDataFileName);

	SOUND_MANAGER->PlaySound("StartLongBGM");
	return true;
}

void InHouseScene::Input(float dt)
{
	GameScene::Input(dt);
}

void InHouseScene::Update(float dt)
{
	GameScene::Update(dt);
}

void InHouseScene::LateUpdate(float dt)
{
	GameScene::LateUpdate(dt);

	Pos curPos = AccessPlayer()->GetCenterPos();
	TILE_OPTION option = GetTileOption(curPos);
	switch (option)
	{
	case TO_BEACON_1:
		SceneState state;
		state.nextDir = DOWN;
		state.nextBeacon = BC_ONE;
		state.nextScene = SC_FARM;
		SCENE_MANAGER->SignalizeSceneChange(state);
		break;
	}
}

void InHouseScene::Collision(float dt)
{
	GameScene::Collision(dt);
}

void InHouseScene::Draw(HDC hdc, float dt)
{
	GameScene::Draw(hdc, dt);
}

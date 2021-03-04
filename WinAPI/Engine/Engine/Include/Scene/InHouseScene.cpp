#include "InHouseScene.h"
#include "../Object/MoveObj/Player.h"

InHouseScene::InHouseScene()
{
}

InHouseScene::~InHouseScene()
{
}

bool InHouseScene::Init()
{
	Layer* pLayer = FindLayer("Object");

	Object* pPlayer = Object::CreateObject<Player>("Player", pLayer);

	GameScene::SetUpScene(pGameDataFileName, pPlayer, Pos(0.5f, 0.5f));

	SAFE_RELEASE(pPlayer);

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
}

void InHouseScene::Collision(float dt)
{
	GameScene::Collision(dt);
}

void InHouseScene::Draw(HDC hdc, float dt)
{
	GameScene::Draw(hdc, dt);
}

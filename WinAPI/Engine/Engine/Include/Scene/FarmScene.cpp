#include "FarmScene.h"

FarmScene::FarmScene()
{
}

FarmScene::~FarmScene()
{
}

bool FarmScene::Init()
{
	GameScene::SetUpScene(pGameDataFileName);
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
}

void FarmScene::Collision(float dt)
{
	GameScene::Collision(dt);
}

void FarmScene::Draw(HDC hdc, float dt)
{
	GameScene::Draw(hdc, dt);
}

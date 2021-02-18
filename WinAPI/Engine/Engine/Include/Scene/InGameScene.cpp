#include "InGameScene.h"
#include "../Object/Player.h"
#include "../Object/Minion.h"
#include "Layer.h"

InGameScene::InGameScene()
{
}

InGameScene::~InGameScene()
{
}

bool InGameScene::Init()
{
    if (!Scene::Init())
    {
        return false;
    }

    Layer* pLayer = FindLayer("Default");

    Player* pPlayer = Object::CreateObject<Player>("Player", pLayer);
    Minion* pMinion = Object::CreateObject<Minion>("Minion", pLayer);

    return true;
}

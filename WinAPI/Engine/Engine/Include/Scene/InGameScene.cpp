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

    Object* pPlayer = Object::CreateObject<Player>("Player", pLayer);
    Object* pMinion = Object::CreateObject<Minion>("Minion", pLayer);

    SAFE_RELEASE(pPlayer);
    SAFE_RELEASE(pMinion);

    return true;
}

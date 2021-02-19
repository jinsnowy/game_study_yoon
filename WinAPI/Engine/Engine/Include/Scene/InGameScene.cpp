#include "InGameScene.h"
#include "../Object/Player.h"
#include "../Object/Minion.h"
#include "../Object/Bullet.h"
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
    SAFE_RELEASE(pPlayer);

    Object* pMinion = Object::CreateObject<Minion>("Minion", pLayer);
    SAFE_RELEASE(pMinion);

    Bullet* pBullet = Object::CreatePrototype<Bullet>("Bullet");
    pBullet->SetSize(50.f, 50.f);

    SAFE_RELEASE(pBullet);
    return true;
}

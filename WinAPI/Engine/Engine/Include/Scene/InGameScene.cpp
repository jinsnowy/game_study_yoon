#include "InGameScene.h"
#include "../Object/Player.h"
#include "../Object/Minion.h"
#include "../Object/Bullet.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderPixel.h"
#include "../Core/Camera.h"
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
    CAMERA->SetTarget(pPlayer);
    CAMERA->SetPivot(0.5f, 0.5f);

    SAFE_RELEASE(pPlayer);

    Object* pMinion = Object::CreateObject<Minion>("Minion", pLayer);

    SAFE_RELEASE(pMinion);

    Bullet* pBullet = Scene::CreatePrototype<Bullet>("Bullet", m_eSceneType);
    pBullet->SetSize(50.f, 50.f);

    SAFE_RELEASE(pBullet);

    Layer* pStageLayer = FindLayer("Stage");

    Stage* pStage = Object::CreateObject<Stage>("Stage", pStageLayer);

    ColliderPixel* pPixel = pStage->AddCollider<ColliderPixel>("StageColl");

    pPixel->SetPixelInfo("MaskStage2.bmp");

    SAFE_RELEASE(pPixel);

    SAFE_RELEASE(pStage);
    
    return true;
}

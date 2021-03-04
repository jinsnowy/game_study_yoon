#include "GameScene.h"
#include "../Object/StaticObj/Stage.h"
#include "../Object/Object.h"
#include "../Core/PathManager.h"
#include "../Core/Camera.h"
#include "Layer.h"

/*
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
*/

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}


bool GameScene::Init()
{
    return true;
}

void GameScene::Input(float dt)
{
    Scene::Input(dt);
}

void GameScene::Update(float dt)
{
    Scene::Update(dt);
}

void GameScene::LateUpdate(float dt)
{
    Scene::LateUpdate(dt);
}

void GameScene::Collision(float dt)
{
    Scene::Collision( dt);
}

void GameScene::Draw(HDC hdc, float dt)
{
    Scene::Draw(hdc, dt);
}

void GameScene::SetUpScene(const char* fileName, Object* pPlayer, Pos camPivot)
{
    LoadDefaultStages(fileName);
    Stage* pStage = static_cast<Stage*>(Object::FindObject("Ground"));
    Size imgSize = pStage->GetImageSize();
  
    int worldX = pStage->GetStageWidth() > imgSize.x ? pStage->GetStageWidth() : imgSize.x;
    int worldY = pStage->GetStageHeight() > imgSize.y ? pStage->GetStageHeight() : imgSize.y;

    CAMERA->SetWorldResolution(worldX, worldY);
    CAMERA->SetPos(0.f, 0.f);
    CAMERA->SetPivot(camPivot);
    CAMERA->SetTarget(pPlayer);
}

void GameScene::LoadStage(STAGE_TAG eStageTag, const string& strlayerTag, FILE* pFile)
{
    Layer* pStageLayer = FindLayer(strlayerTag);
    Stage* pStage = Object::CreateObject<Stage>(strlayerTag, pStageLayer);
    pStage->LoadFromFile(pFile);
    SAFE_RELEASE(pStage);
}

void GameScene::LoadDefaultStages(const char* fileName)
{
    FILE* pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "rb");

    LoadStage(ST_GROUND, "Ground", pFile);
    LoadStage(ST_STATIC, "Static", pFile);

    if (pFile)
    {
        fclose(pFile);
    }
}

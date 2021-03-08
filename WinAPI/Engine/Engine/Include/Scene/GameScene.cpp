#include "GameScene.h"
#include "../Object/StaticObj/Stage.h"
#include "../Object/Object.h"
#include "../Core/PathManager.h"
#include "../Core/Camera.h"
#include "Layer.h"

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
    Stage* pStage = static_cast<Stage*>(Object::FindObject("GroundStage"));
    Size imgSize = pStage->GetImageSize();
  
    int worldX = pStage->GetStageWidth() > imgSize.x ? pStage->GetStageWidth() : imgSize.x;
    int worldY = pStage->GetStageHeight() > imgSize.y ? pStage->GetStageHeight() : imgSize.y;

    CAMERA->SetWorldResolution(worldX, worldY);
    CAMERA->SetPos(0.f, 0.f);
    CAMERA->SetPivot(camPivot);
    CAMERA->SetTarget(pPlayer);
}

void GameScene::LoadStage(const string& objectTag, const string& strlayerTag,  FILE* pFile)
{
    Layer* pStageLayer = FindLayer(strlayerTag);
    Stage* pStage = Object::CreateObject<Stage>(objectTag, pStageLayer);
    pStage->LoadFromFile(pFile);
    SAFE_RELEASE(pStage);
}

void GameScene::LoadDefaultStages(const char* fileName)
{
    FILE* pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "rb");

    LoadStage("GroundStage", "Ground", pFile);
    LoadStage("ObjectStage", "", pFile);
    LoadStage("StaticStage", "Static", pFile);

    if (pFile)
    {
        fclose(pFile);
    }

    Stage* pStage = static_cast<Stage*>(Object::FindObject("ObjectStage"));
    pStage->AddAllTilesInLayer(FindLayer("Object"));
    Object::EraseObject(Object::FindObject("ObjectStage"));
}

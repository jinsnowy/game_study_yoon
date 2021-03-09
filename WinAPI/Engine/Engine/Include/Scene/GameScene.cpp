#include "GameScene.h"
#include "../Object/StaticObj/Stage.h"
#include "../Resources/PrototypeManager.h"
#include "../Object/Object.h"
#include "../Core/PathManager.h"
#include "../Core/Camera.h"
#include "Layer.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
    SAFE_RELEASE(m_pBaseStage);
}

int GameScene::GetTileIndex(const Pos& worldPos)
{
    return m_pBaseStage->GetTileIndex(worldPos);
}
INDEX GameScene::GetTileRowColIndex(const Pos& worldPos)
{
    return m_pBaseStage->GetTileRowColIndex(worldPos);
}

INDEX GameScene::IndexDiff(const Pos& pos, const Pos& from)
{
    return GetTileRowColIndex(pos) - GetTileRowColIndex(from);
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
    Stage* pStage = static_cast<Stage*>(FindObject("GroundStage"));
    Size imgSize = pStage->GetImageSize();
  
    float worldX = pStage->GetStageWidth() > imgSize.x ? pStage->GetStageWidth() : imgSize.x;
    float worldY = pStage->GetStageHeight() > imgSize.y ? pStage->GetStageHeight() : imgSize.y;

    CAMERA->SetWorldResolution(int(worldX), int(worldY));
    CAMERA->SetPos(0.f, 0.f);
    CAMERA->SetPivot(camPivot);
    CAMERA->SetTarget(pPlayer);
}

void GameScene::LoadStage(const string& objectTag, const string& strlayerTag,  FILE* pFile)
{
    Layer* pStageLayer = FindLayer(strlayerTag);
    Stage* pStage = CreateObject<Stage>(objectTag, pStageLayer);
    pStage->LoadFromFile(pFile);
    SAFE_RELEASE(pStage);
}

void GameScene::LoadDefaultStages(const char* fileName)
{
    FILE* pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "rb");

    LoadStage("GroundStage", "Ground", pFile);
    LoadStage("StaticStage", "Static", pFile);

    Layer* objLayer = FindLayer("Object");
    size_t objNum;
    fread(&objNum, sizeof(objNum), 1, pFile);
    if (objNum > 0)
    {
        int length = 0;
        char strTag[MAX_PATH] = { 0 };
        bool hasPrototype = false;
        Object* pObj = nullptr;
        for (int i = 0; i < objNum; ++i)
        {
            fread(&hasPrototype, 1, 1, pFile);
            if (hasPrototype)
            {
                fread(&length, 4, 1, pFile);
                fread(strTag, 1, length, pFile);
                strTag[length] = 0;
                string prototypeKey = string(strTag);
                pObj = PROTOTYPE_MANAGER->CreateCloneObject(prototypeKey, strTag, this, objLayer);
                pObj->Load(pFile);
                SAFE_RELEASE(pObj);
            }
        }
    }

    if (pFile)
    {
        fclose(pFile);
    }

    m_pBaseStage = static_cast<Stage*>(FindObject("GroundStage"));
    m_pBaseStage->AddRef();
    m_iTileNumX = m_pBaseStage->GetStageTileNumX();
    m_iTileNumY = m_pBaseStage->GetStageTileNumY();
}

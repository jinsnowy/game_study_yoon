#include "GameScene.h"
#include "../Object/StaticObj/Stage.h"
#include "../Resources/PrototypeManager.h"
#include "../Object/Object.h"
#include "../Core/PathManager.h"
#include "../Object/StaticObj/Tile.h"
#include "../Object/StaticObj/Tree.h"
#include "../Core/Camera.h"
#include "../Object/MoveObj/Player.h"
#include "Layer.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
    SAFE_RELEASE(m_pGroundStage);
    SAFE_RELEASE(m_pStaticStage);
    SAFE_RELEASE(m_pPlayer);
}

Pos GameScene::FindBeacon(BEACON_TAG bc)
{
    TILE_OPTION option = TO_NONE;
    if (bc == BC_ONE)
        option = TO_BEACON_1;
    else if (bc == BC_TWO)
        option = TO_BEACON_2;
    else if (bc == BC_THREE)
        option = TO_BEACON_3;

    const auto tiles = m_pStaticStage->GetTiles();
    int found = -1;
    int size = (int) tiles->size();
    for (int i =0;i<size;++i)
    {
        if ((*tiles)[i]->GetTileOption() == option)
        {
            found = i;
            break;
        }
    }
    assert(found != -1 && option != TO_NONE);

    return m_pStaticStage->GetTilePos(found);
}

void GameScene::SetUpMainCharacter(Player* pPlayer)
{
    m_pPlayer = pPlayer;
    m_pPlayer->AddRef();

    AddObject(pPlayer);
    FindLayer("Object")->AddObject(pPlayer);

    m_pPlayer->LateInit();
}

void GameScene::SpawnMainCharacter(const Pos& worldPos)
{
    m_pPlayer->SetPos(worldPos);
}

int GameScene::GetTileIndex(const Pos& worldPos)
{
    return m_pGroundStage->GetTileIndex(worldPos);
}
Pos GameScene::GetTilePos(const Pos& worldPos)
{
    return m_pStaticStage->GetTilePos(worldPos);
}
INDEX GameScene::GetTileRowColIndex(const Pos& worldPos)
{
    return m_pGroundStage->GetTileRowColIndex(worldPos);
}

INDEX GameScene::IndexDiff(const Pos& pos, const Pos& from)
{
    return GetTileRowColIndex(pos) - GetTileRowColIndex(from);
}


TILE_OPTION GameScene::GetTileOption(const Pos& worldPos)
{
    return m_pStaticStage->GetTileOption(worldPos);
}

bool GameScene::IsBlockTile(const Pos& worldPos)
{
    return m_pStaticStage->IsBlockTile(worldPos);
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

void GameScene::SetUpScene(SceneState state, Player* player)
{
    if (m_pPlayer == nullptr)
    {
        SetUpMainCharacter(player);
    }

    if (state.nextBeacon != BC_NONE)
    {
        Pos tPos = FindBeacon(state.nextBeacon);
        SpawnMainCharacter(tPos);
    }
    switch (state.nextDir)
    {
    case LEFT:
        m_pPlayer->StateTransit(Player::PlayerState::IDLE_LEFT);
        break;
    case RIGHT:
        m_pPlayer->StateTransit(Player::PlayerState::IDLE_RIGHT);
        break;
    case UP:
        m_pPlayer->StateTransit(Player::PlayerState::IDLE_RIGHT);
        break;
    case DOWN:
        m_pPlayer->StateTransit(Player::PlayerState::IDLE_DOWN);
        break;
    }

    float worldX = m_pStaticStage->GetStageWidth();
    float worldY = m_pStaticStage->GetStageHeight();

    CAMERA->SetWorldResolution(int(worldX), int(worldY));
    CAMERA->SetTarget(m_pPlayer);
    CAMERA->SetPivot(0.5f, 0.5f);
    CAMERA->SetPos(m_pPlayer->GetPos());
}

void GameScene::SetUpScene(const char* fileName)
{
    LoadDefaultStages(fileName);
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
        int objType = 0;
        bool hasPrototype = false;
        Object* pObj = nullptr;
        for (int i = 0; i < objNum; ++i)
        {
            fread(&objType, 4, 1, pFile);
            fread(&hasPrototype, 1, 1, pFile);
            if (hasPrototype)
            {
                fread(&length, 4, 1, pFile);
                fread(strTag, 1, length, pFile);
                strTag[length] = 0;
                string prototypeKey = string(strTag);
                pObj = PROTOTYPE_MANAGER->FindPrototype(prototypeKey)->Clone();
            }
            else {
                switch (objType)
                {
                case OBJ_TREE:
                    pObj = Object::CreateObject<Tree>("Tree");
                    break;
                case OBJ_TILE:
                    pObj = Object::CreateObject<Tile>("Tile");
                    break;
                }
            }
            pObj->Load(pFile);
            objLayer->AddObject(pObj);
            SAFE_RELEASE(pObj);
        }
    }

    if (pFile)
    {
        fclose(pFile);
    }

    m_pGroundStage = static_cast<Stage*>(FindObject("GroundStage"));
    m_pGroundStage->AddRef();

    m_pStaticStage = static_cast<Stage*>(FindObject("StaticStage"));
    m_pStaticStage->AddRef();

    m_iTileNumX = m_pGroundStage->GetStageTileNumX();
    m_iTileNumY = m_pGroundStage->GetStageTileNumY();
}

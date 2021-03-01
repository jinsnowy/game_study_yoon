#include "Stage.h"
#include "Tile.h"
#include "../../Resources/Texture.h"
#include "../../Resources/ResourceManager.h"
#include "../../framework.h"
#include "../../Core/Camera.h"
#include "../../Application/Window.h"
#include "../../Core/PathManager.h"

Stage::Stage()
{
}

Stage::Stage(const Stage& stage)
    :
    StaticObject(stage)
{
    m_groundTile.clear();

    for (size_t i = 0; i < stage.m_groundTile.size(); ++i)
    {
        m_groundTile.push_back(stage.m_groundTile[i]->Clone());
    }
}

Stage::~Stage()
{
    ClearTile();
}

void Stage::DrawBackGround(HDC hdc, COLORREF color)
{
    RESOLUTION tPos = CAMERA->GetWorldRS();

    HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(color));

    Rectangle(hdc, 0, 0, tPos.x, tPos.y);

    DeleteObject(SelectObject(hdc, OldBrush));
}

void Stage::CreateTile(int iNumX, int iNumY, int iSizeX, int iSizeY, const string& strKey, const wchar_t* pFileName, const string& strPathKey)
{
    SAFE_RELEASE(m_baseTexture);
    Safe_Release_VecList(m_groundTile);

    m_iTileNumX = iNumX;
    m_iTileNumY = iNumY;
    m_iTileSizeX = iSizeX;
    m_iTileSizeY = iSizeY;
    m_baseTexture = RESOURCE_MANAGER->LoadTexture(strKey, pFileName);
    Pos offset;
    for (int i = 0; i < iNumY; ++i)
    {
        for (int j = 0; j < iNumX; ++j)
        {
            Tile* pTile = Object::CreateObject<Tile>("Tile");
            
            offset.x = j * iSizeX;
            offset.y = i * iSizeY;

            pTile->SetSize(iSizeX, iSizeY);
            pTile->SetPos(offset.x, offset.y);
            pTile->SetTexture(strKey, pFileName, strPathKey);
            m_groundTile.push_back(pTile);
        }
    }
}

bool Stage::Init()
{
    //SetPos(0.0f, 0.0f);
    //SetSize(1920.f, 1080.f);
    //SetPivot(0.0f, 0.0f);

    //SetTexture("Stage2", L"MaskStage2.bmp");

    return true;
}

void Stage::Input(float dt)
{
    StaticObject::Input(dt);
}

int Stage::Update(float dt)
{
    StaticObject::Update(dt);
    return 0;
}

int Stage::LateUpdate(float dt)
{
    StaticObject::LateUpdate(dt);
    return 0;
}

void Stage::Collision(float dt)
{
    StaticObject::Collision(dt);
}

void Stage::Draw(HDC hDC, float dt)
{
    DrawBackGround(hDC, RGB(73, 139, 97));

    for (size_t i = 0; i < m_groundTile.size(); ++i)
    {
        m_groundTile[i]->Draw(hDC, dt);
    }

    // Grid를 그린다.
    Pos tCamPos = CAMERA->GetPos();
    for (int i = 1; i <= m_iTileNumY; ++i)
    {
        // 가로줄을 그린다.
        MoveToEx(hDC, 0, i * m_iTileSizeY - tCamPos.y, NULL);
        LineTo(hDC, m_iTileNumX * m_iTileSizeX - tCamPos.x, i * m_iTileSizeY - tCamPos.y);
    }

    // 세로줄을 그린다.
    for (int i = 1; i <= m_iTileNumX; ++i)
    {
        MoveToEx(hDC, i * m_iTileSizeX - tCamPos.x, 0, NULL);
        LineTo(hDC, i * m_iTileSizeX - tCamPos.x, m_iTileNumY * m_iTileSizeY - tCamPos.y);
    }
}

Stage* Stage::Clone()
{
    return new Stage(*this);
}


void Stage::Save(FILE* pFile)
{
    StaticObject::Save(pFile);

    // 스테이지 정보 저장
    fwrite(&m_iTileNumX, 4, 1, pFile);
    fwrite(&m_iTileNumY, 4, 1, pFile);
    fwrite(&m_iTileSizeX, 4, 1, pFile);
    fwrite(&m_iTileSizeY, 4, 1, pFile);

    for (size_t i = 0; i < m_groundTile.size(); ++i)
    {
        m_groundTile[i]->Save(pFile);
    }
}

void Stage::Load(FILE* pFile)
{
    StaticObject::Load(pFile);

    // 스테이지 정보 저장
    fread(&m_iTileNumX, 4, 1, pFile);
    fread(&m_iTileNumY, 4, 1, pFile);
    fread(&m_iTileSizeX, 4, 1, pFile);
    fread(&m_iTileSizeY, 4, 1, pFile);
    
    ClearTile();

    for (int i = 0; i < m_iTileNumX * m_iTileNumY; ++i)
    {
        Tile* pTile = Object::CreateObject<Tile>("Tile");

        pTile->Load(pFile);

        m_groundTile.push_back(pTile);
    }
}

void Stage::ChangeTileTexture(const Pos& tPos, Texture* pTexture)
{
    int ind = GetTileIndex(tPos);

    if (ind == -1)
        return;

    m_groundTile[ind]->SetTexture(pTexture);
}

void Stage::ChangeTileOption(const Pos& tPos, TILE_OPTION eOption)
{
    int ind = GetTileIndex(tPos);

    if (ind == -1)
        return;

    m_groundTile[ind]->SetTileOption(eOption);
}

int Stage::GetTileIndex(const Pos& tPos)
{
    return GetTileIndex(tPos.x ,tPos.y);
}

int Stage::GetTileIndex(float x, float y)
{
    int idxX = (int)x / m_iTileSizeX;
    int idxY = (int)y / m_iTileSizeY;

    if (idxX < 0 || idxX >= m_iTileNumX || idxY < 0 || idxY >= m_iTileNumY)
        return -1;

    return idxY * m_iTileNumX + idxX;
}

void Stage::ClearTile()
{
    SAFE_RELEASE(m_baseTexture);
    for (size_t i = 0; i < m_groundTile.size(); ++i)
    {
        Object::EraseObject(m_groundTile[i]);
    }
    Safe_Release_VecList(m_groundTile);
}

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
    m_baseTile.clear();

    for (size_t i = 0; i < stage.m_baseTile.size(); ++i)
    {
        m_baseTile.push_back(stage.m_baseTile[i]->Clone());
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

void Stage::CreateTile(int iNumX, int iNumY, int iSizeX, int iSizeY, Pos tPivot)
{
    Safe_Release_VecList(m_baseTile);

    m_iTileNumX = iNumX;
    m_iTileNumY = iNumY;
    m_iTileSizeX = iSizeX;
    m_iTileSizeY = iSizeY;
    Pos offset;
    for (int i = 0; i < iNumY; ++i)
    {
        for (int j = 0; j < iNumX; ++j)
        {
            Tile* pTile = Object::CreateObject<Tile>("Tile", nullptr);
            
            offset.x = j * iSizeX;
            offset.y = i * iSizeY;

            pTile->SetSize(iSizeX, iSizeY);
            pTile->SetPos(offset.x, offset.y);
            pTile->SetPivot(tPivot);
            m_baseTile.push_back(pTile);
        }
    }
}

bool Stage::Init()
{
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
    for (size_t i = 0; i < m_baseTile.size(); ++i)
    {
        m_baseTile[i]->Draw(hDC, dt);
    }

#ifdef _DEBUG
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
#endif
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

    for (size_t i = 0; i < m_baseTile.size(); ++i)
    {
        m_baseTile[i]->Save(pFile);
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
        Tile* pTile = Object::CreateObject<Tile>("Tile", nullptr);

        pTile->Load(pFile);

        m_baseTile.push_back(pTile);
    }
}

TILE_OPTION Stage::GetTileOption(const Pos& pos) const
{
    int ind = GetTileIndex(pos);
    if (ind == -1)
        return TO_NONE;
    return m_baseTile[ind]->m_eOption;
}
string Stage::GetTileName(const Pos& pos) const
{
    int ind = GetTileIndex(pos);
    if (ind == -1)
        return "";

    if(m_baseTile[ind]->m_pTexture)
        return m_baseTile[ind]->m_pTexture->GetTag();
    return "";
}

void Stage::SetTileNone(const Pos& tPos)
{
    int ind = GetTileIndex(tPos);

    if (ind == -1)
        return;
    m_baseTile[ind]->ReleaseTexture();
}

void Stage::ChangeTileTexture(const Pos& tPos, Texture* pTexture)
{
    int ind = GetTileIndex(tPos);

    if (ind == -1)
        return;

    m_baseTile[ind]->SetTexture(pTexture);
}

void Stage::ChangeTileOption(const Pos& tPos, TILE_OPTION eOption)
{
    int ind = GetTileIndex(tPos);

    if (ind == -1)
        return;

    m_baseTile[ind]->SetTileOption(eOption);
}

int Stage::GetTileIndex(const Pos& tPos) const
{
    return GetTileIndex(tPos.x ,tPos.y);
}

int Stage::GetTileIndex(float x, float y) const
{
    int idxX = (int)x / m_iTileSizeX;
    int idxY = (int)y / m_iTileSizeY;

    if (idxX < 0 || idxX >= m_iTileNumX || idxY < 0 || idxY >= m_iTileNumY)
        return -1;

    return idxY * m_iTileNumX + idxX;
}

INDEX Stage::GetTileRowColIndex(const Pos& tPos) const
{
    int index = GetTileIndex(tPos);
    if (index == -1) return { -1,-1 };
    return INDEX(index / m_iTileNumY, index % m_iTileNumY);
}

INDEX Stage::GetTileRowColIndex(float x, float y) const
{
    int index = GetTileIndex(x, y);
    if (index == -1) return { -1,-1 };
    return INDEX(index / m_iTileNumY, index % m_iTileNumY);
}

void Stage::ClearTile()
{
    for (int i = 0; i < m_baseTile.size(); ++i)
    {
        Object::EraseObject(m_baseTile[i]);
    }
    Safe_Release_VecList(m_baseTile);
}

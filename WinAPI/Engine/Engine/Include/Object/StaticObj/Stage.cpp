#include "Stage.h"
#include "Tile.h"
#include "../../Resources/Texture.h"
#include "../../framework.h"
#include "../../Core/Camera.h"
#include "../../Application/Window.h"

Stage::Stage()
{
}

Stage::Stage(const Stage& stage)
    :
    StaticObject(stage)
{
    m_vecTile.clear();

    for (size_t i = 0; i < stage.m_vecTile.size(); ++i)
    {
        m_vecTile.push_back(stage.m_vecTile[i]->Clone());
    }
}

Stage::~Stage()
{
    Safe_Release_VecList(m_vecTile);
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
    Safe_Release_VecList(m_vecTile);

    m_iTileNumX = iNumX;
    m_iTileNumY = iNumY;
    m_iTileSizeX = iSizeX;
    m_iTileSizeY = iSizeY;

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
            pTile->SetImageOffset(offset.x, offset.y);
            m_vecTile.push_back(pTile);

            SAFE_RELEASE(pTile);
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
    //// StaticObject::Draw(hDC, dt);
    //if (m_pTexture)
    //{
    //    Pos tPos = m_tPos - m_tPivot * m_tSize;
    //    Pos tCamTopLeft = CAMERA->GetTopLeft();
    //    BitBlt(hDC, int(tPos.x), int(tPos.y),
    //                CAMERA->GetClientRS().x, CAMERA->GetClientRS().y,
    //                m_pTexture->GetDC(),
    //                int(tCamTopLeft.x), int(tCamTopLeft.y),
    //                SRCCOPY);
    //}

    DrawBackGround(hDC, RGB(73, 139, 97));

    for (size_t i = 0; i < m_vecTile.size(); ++i)
    {
        m_vecTile[i]->Draw(hDC, dt);
    }
}

Stage* Stage::Clone()
{
    return new Stage(*this);
}

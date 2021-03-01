#include "UITileSelect.h"
#include "../../Core/PathManager.h"
#include "../../Resources/Texture.h"
#include "../../Resources/ResourceManager.h"
#include <cassert>

UITileSelect::UITileSelect()
{
    m_BaseTiles.resize(size_t(UISEL_TYPE::SEL_END));
}

UITileSelect::~UITileSelect()
{
    for (auto& tiles : m_BaseTiles)
    {
        Safe_Release_VecList(tiles);
    }
}

Texture* UITileSelect::SelectTile(const Pos& screenPos)
{
    Pos tPos = GetPos();
    Pos tSize = GetSize();

    int st_x = tPos.x + (tSize.x - totalSizeX) / 2;
    int st_y = tPos.y + (tSize.y - totalSizeY) / 2;

    int px = st_x, py = st_y, itemInd = 0;
    for (int j = 0; j < itemNumY; ++j)
    {
        for (int i = 0; i < itemNumX; ++i)
        {
            if (screenPos.x >= px && screenPos.x < px + TILESIZE
                && screenPos.y >= py && screenPos.y < py + TILESIZE)
            {
                m_BaseTiles[m_iCurSel][itemInd]->AddRef();
                return m_BaseTiles[m_iCurSel][itemInd];
            }
            ++itemInd;
            px += (TILESIZE + m_iMarginItem);
        }
        px = st_x;
        py += (TILESIZE + m_iMarginItem);
    }

    int pageNum = m_BaseTiles[m_iCurSel].size() / (itemNumY * itemNumX) + 1;

    px = st_x; py = tPos.y + tSize.y - m_iSelButtonSize - 20;
    for (int i = 0; i < pageNum; i++)
    {
        if (screenPos.x >= px && screenPos.x < px + m_iSelButtonSize
            && screenPos.y >= py && screenPos.y < py + m_iSelButtonSize)
        {
            m_iCurPageNum = i;
            return nullptr;
        }
        px += (m_iSelButtonSize + m_iMarginItem);
    }

    return nullptr;
}

void UITileSelect::LoadTiles(UISEL_TYPE eSel, const wchar_t* pBaseFolderName, const string& strPathKey)
{
    const wchar_t* pPath = PATH_MANAGER->FindPath(strPathKey);
    wstring strPath;
    if (pPath)
        strPath = pPath;

    strPath += pBaseFolderName;
    assert(strPath.back() == L'\\' || strPath.back() == L'/');

    const auto extract_key = [](const wchar_t* str, int size)
    {
        int ed = size - 1;
        while (str[ed] != L'.') --ed;
        int st = ed - 1;
        while (str[st] != L'\\' && str[st] != L'/') st--;
        return string(str + st+1, str + ed);
    };
    
    for (const auto& entry : fs::directory_iterator(strPath))
    {
        const wchar_t * path = entry.path().c_str();
        string strkey = extract_key(path, lstrlen(path));
        Texture * pTex = RESOURCE_MANAGER->LoadTexture(strkey, path, "");
        pTex->SetColorKey(255, 255, 255);
        m_BaseTiles[int(eSel)].push_back(pTex);
    }
}

bool UITileSelect::Init()
{
    return true;
}
void UITileSelect::SetMargin(int w, int h)
{
     m_iMarginWidth = w; m_iMarginHeight = h;
     Pos tSize = GetSize();
     validWidth = tSize.x - 2 * m_iMarginWidth + m_iMarginItem;
     validHeight = tSize.y - 2 * m_iMarginHeight + m_iMarginItem;
     itemNumX = validWidth / (TILESIZE + m_iMarginItem);
     itemNumY = validHeight / (TILESIZE + m_iMarginItem);
     totalSizeX = itemNumX * TILESIZE + (itemNumX - 1) * m_iMarginItem;
     totalSizeY = itemNumY * TILESIZE + (itemNumY - 1) * m_iMarginItem;
}
void UITileSelect::Input(float dt)
{
    UI::Input(dt);
}

int UITileSelect::Update(float dt)
{
    UI::Update(dt);
    return 0;
}

int UITileSelect::LateUpdate(float dt)
{
    UI::LateUpdate(dt);
    return 0;
}

void UITileSelect::Collision(float dt)
{
    UI::Collision(dt);
}

void UITileSelect::Draw(HDC hdc, float dt)
{
    assert(m_iCurSel < int(UISEL_TYPE::SEL_END));
    UI::Draw(hdc, dt);

    Pos tPos = GetPos();
    Pos tSize = GetSize();

    if (itemNumX == 0 || itemNumY == 0)
    {
        SetMargin(m_iMarginWidth, m_iMarginHeight);
    }

    int st_x = tPos.x + (tSize.x - totalSizeX) / 2;
    int st_y = tPos.y + (tSize.y - totalSizeY) / 2;
    int tileNum = m_BaseTiles[m_iCurSel].size();

    int px = st_x, py = st_y, itemInd = m_iCurPageNum * itemNumX * itemNumY;
    for (int j = 0; j < itemNumY && itemInd < tileNum; j++)
    {
        for (int i = 0; i < itemNumX && itemInd < tileNum; i++)
        {
            m_BaseTiles[m_iCurSel][itemInd++]->TileDraw(hdc, px, py);
            px += (TILESIZE + m_iMarginItem);
        }
        px = st_x;
        py += (TILESIZE + m_iMarginItem);
    }

    const int pageNum = tileNum / (itemNumY * itemNumX) + 1;

    px = st_x; py = tPos.y + tSize.y - m_iSelButtonSize - 20;
    for (int i = 0; i < pageNum; i++)
    {
        m_BaseTiles[int(UISEL_TYPE::SEL_NUMBER)][i]->TileDraw(hdc, px, py, m_iSelButtonSize);
        px += (m_iSelButtonSize + m_iMarginItem);
    }
}
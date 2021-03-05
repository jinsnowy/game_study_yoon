#include "UITileSelect.h"
#include "../../Core/PathManager.h"
#include "../../Resources/Texture.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/PrototypeManager.h"
#include "../../Object/Object.h"
#include "../../Core/Input.h"
#include <cassert>

UITileSelect::UITileSelect()
{
    m_BaseTiles.resize(size_t(SEL_END), vector<Texture*>());
    m_PrototypeMapContainer.resize(size_t(OBJ_END), nullptr);
}

UITileSelect::~UITileSelect()
{
    for (auto& tiles : m_BaseTiles)
    {
        Safe_Release_VecList(tiles);
    }
    m_BaseTiles.clear();
    m_PrototypeMapContainer.clear();
    INPUT->DeleteKey("NextObject");
    INPUT->DeleteKey("PreviousObject");
}

void UITileSelect::InitUI()
{
    Pos tSize = GetSize();
    assert(tSize.x > 0 && tSize.y > 0);
    m_iValidWidth = tSize.x - 2 * m_iMarginWidth + m_iMarginItem;
    m_iValidHeight = tSize.y - 2 * m_iMarginHeight + m_iMarginItem;
    m_iDrawMaxitemNumX = m_iValidWidth / (TILESIZE + m_iMarginItem);
    m_iDrawMaxitemNumY = m_iValidHeight / (TILESIZE + m_iMarginItem);
    m_iTotalSizeX = m_iDrawMaxitemNumX * TILESIZE + (m_iDrawMaxitemNumX - 1) * m_iMarginItem;
    m_iTotalSizeY = m_iDrawMaxitemNumY * TILESIZE + (m_iDrawMaxitemNumY - 1) * m_iMarginItem;
}

void UITileSelect::SetCurSelect(UISEL_TYPE eSel)
{
    m_iCurPageNum = 0;
    m_eCurSelTile = eSel;
    if(eSel == SEL_OBJECT)
    {
        m_eCurSelObject = OBJ_TILE_INNER;
    }
}

bool UITileSelect::IsObjectTileMode() const
{
    switch (m_eCurSelObject)
    {
    case OBJ_FREE:
        return false;
    }
    return true;
}

Texture* UITileSelect::SelectTile(const Pos& screenPos)
{
    Pos tPos = GetPos();
    Pos tSize = GetSize();

    int st_x = tPos.x + (tSize.x - m_iTotalSizeX) / 2;
    int st_y = tPos.y + (tSize.y - m_iTotalSizeY) / 2;

    int px = st_x, py = st_y, itemInd = m_iCurPageNum * m_iDrawMaxitemNumX * m_iDrawMaxitemNumY;
    for (int j = 0; j < m_iDrawMaxitemNumY; ++j)
    {
        for (int i = 0; i < m_iDrawMaxitemNumX; ++i)
        {
            if (screenPos.x >= px && screenPos.x < px + TILESIZE
                && screenPos.y >= py && screenPos.y < py + TILESIZE)
            {
                m_BaseTiles[m_eCurSelTile][itemInd]->AddRef();
                return m_BaseTiles[m_eCurSelTile][itemInd];
            }
            ++itemInd;
            px += (TILESIZE + m_iMarginItem);
        }
        px = st_x;
        py += (TILESIZE + m_iMarginItem);
    }
    return nullptr;
}


bool UITileSelect::SelectUITag(const Pos& screenPos)
{
    Pos tPos = GetPos();
    Pos tSize = GetSize();

    // 페이지 클릭
    int itemNum;
    if (m_eCurSelTile == SEL_OBJECT)
    {
        itemNum = (int) m_PrototypeMapContainer[m_eCurSelObject]->size();
    }
    else {
        itemNum = (int)m_BaseTiles[m_eCurSelTile].size();
    }
    int pageNum = itemNum / (m_iDrawMaxitemNumY * m_iDrawMaxitemNumX) + 1;

    int px = tPos.x, py = tPos.y;
    for (int i = 0; i < pageNum; i++)
    {
        if (screenPos.x >= px && screenPos.x < px + m_iSelButtonSize
            && screenPos.y >= py && screenPos.y < py + m_iSelButtonSize)
        {
            m_iCurPageNum = i;
            return true;
        }
        px += (m_iSelButtonSize + m_iMarginItem);
    }

    if (screenPos.x >= px && screenPos.x < px + tSize.x
        && screenPos.y >= py && screenPos.y < py + tSize.y)
        return true;

    return false;
}

Object* UITileSelect::SelectObject(const Pos& screenPos)
{
    const unordered_map<string, Object*> prototypes = *m_PrototypeMapContainer[m_eCurSelObject];

    unordered_map<string, Object*>::const_iterator iter = prototypes.begin();
    unordered_map<string, Object*>::const_iterator iterEnd = prototypes.end();

    Pos tPos = GetPos();
    Pos tSize = GetSize();
    int st_x = tPos.x + (tSize.x - m_iTotalSizeX) / 2;
    int st_y = tPos.y + (tSize.y - m_iTotalSizeY) / 2;

    int px = st_x, py = st_y;
    int itemInd = m_iCurPageNum * m_iDrawMaxitemNumX * m_iDrawMaxitemNumY;
    for (int j = 0; j < m_iDrawMaxitemNumY && iter != iterEnd; ++j)
    {
        for (int i = 0; i < m_iDrawMaxitemNumX && iter != iterEnd; ++i)
        {
            if (screenPos.x >= px && screenPos.x < px + TILESIZE
                && screenPos.y >= py && screenPos.y < py + TILESIZE)
            {
                return iter->second->Clone();
            }
            ++iter;
            px += (TILESIZE + m_iMarginItem);
        }
        px = st_x;
        py += (TILESIZE + m_iMarginItem);
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

    const auto extract_key = [](const char* str, int size)
    {
        int ed = size - 1;
        while (str[ed] != L'.') --ed;
        int st = ed - 1;
        while (str[st] != L'\\' && str[st] != L'/') st--;
        return string(str + st+1, str + ed);
    };

    Texture* pTex;
    for (const auto& entry : fs::directory_iterator(strPath))
    {
        const wchar_t * path = entry.path().c_str();
        string strkey = extract_key(GetChar(path), lstrlen(path));
        pTex = RESOURCE_MANAGER->LoadTexture(strkey, path, "");
        pTex->SetColorKey(255, 255, 255);
        m_BaseTiles[int(eSel)].push_back(pTex);
    }
}

void UITileSelect::LoadPrototypes(OBJECT_TYPE eType)
{
    m_PrototypeMapContainer[eType] = PROTOTYPE_MANAGER->GetPrototypes(eType);
}

bool UITileSelect::Init()
{
    INPUT->AddKey("NextObject", 'E');
    INPUT->AddKey("PreviousObject", 'Q');
    return true;
}

void UITileSelect::Input(float dt)
{
    UI::Input(dt);
    if (KEYDOWN("NextObject"))
    {
        m_eCurSelObject = static_cast<OBJECT_TYPE>((m_eCurSelObject + 1) % OBJ_END);
    }
    if (KEYDOWN("PreviousObject"))
    {
        m_eCurSelObject = static_cast<OBJECT_TYPE>((m_eCurSelObject + OBJ_END - 1) % OBJ_END);
    }
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
    assert(int(m_eCurSelTile) < int(UISEL_TYPE::SEL_END));
    UI::Draw(hdc, dt);

    switch (m_eCurSelTile)
    {
    case SEL_GROUND:
    case SEL_STATIC:
    case SEL_OPTION:
        DrawTilePanel(hdc, dt);
        break;
    case SEL_OBJECT:
        DrawObjectPanel(hdc, dt);
        break;
    }
}

void UITileSelect::DrawTilePanel(HDC hdc, float dt)
{
    // 타일 번호
    Pos tPos = GetPos();
    Pos tSize = GetSize();
    int st_x = tPos.x + (tSize.x - m_iTotalSizeX) / 2;
    int st_y = tPos.y + (tSize.y - m_iTotalSizeY) / 2;
    int tileNum = m_BaseTiles[int(m_eCurSelTile)].size();

    int px = st_x, py = st_y;
    int itemInd = m_iCurPageNum * m_iDrawMaxitemNumX * m_iDrawMaxitemNumY;
    for (int j = 0; j < m_iDrawMaxitemNumY && itemInd < tileNum; ++j)
    {
        for (int i = 0; i < m_iDrawMaxitemNumX && itemInd < tileNum; ++i)
        {
            m_BaseTiles[int(m_eCurSelTile)][itemInd++]->DrawImageAtFixedSize(hdc, px, py, TILESIZE);
            px += (TILESIZE + m_iMarginItem);
        }
        px = st_x;
        py += (TILESIZE + m_iMarginItem);
    }

    // 페이지 번호
    const int pageNum = tileNum / (m_iDrawMaxitemNumY * m_iDrawMaxitemNumX) + 1;

    px = tPos.x; py = tPos.y;
    for (int i = 0; i < pageNum; i++)
    {
        m_BaseTiles[SEL_NUMBER][i]->DrawImageAtFixedSize(hdc, px, py, m_iSelButtonSize);
        px += (m_iSelButtonSize + m_iMarginItem);
    }
}

void UITileSelect::DrawObjectPanel(HDC hdc, float dt)
{
    const unordered_map<string, Object*> prototypes = *m_PrototypeMapContainer[m_eCurSelObject];

    unordered_map<string, Object*>::const_iterator iter = prototypes.begin();
    unordered_map<string, Object*>::const_iterator iterEnd = prototypes.end();

    Pos tPos = GetPos();
    Pos tSize = GetSize();
    int st_x = tPos.x + (tSize.x - m_iTotalSizeX) / 2;
    int st_y = tPos.y + (tSize.y - m_iTotalSizeY) / 2;

    int px = st_x, py = st_y;
    int itemInd = m_iCurPageNum * m_iDrawMaxitemNumX * m_iDrawMaxitemNumY;
    for (int j = 0; j < m_iDrawMaxitemNumY && iter != iterEnd; ++j)
    {
        for (int i = 0; i < m_iDrawMaxitemNumX && iter != iterEnd; ++i)
        {
            iter->second->AccessTexture()->DrawImageAtFixedSize(hdc, px, py, TILESIZE);
            ++iter;
            px += (TILESIZE + m_iMarginItem);
        }
        px = st_x;
        py += (TILESIZE + m_iMarginItem);
    }

    // 페이지 번호
    const int pageNum = prototypes.size() / (m_iDrawMaxitemNumY * m_iDrawMaxitemNumX) + 1;

    px = tPos.x; py = tPos.y;
    for (int i = 0; i < pageNum; i++)
    {
        m_BaseTiles[SEL_NUMBER][i]->DrawImageAtFixedSize(hdc, px, py, m_iSelButtonSize);
        px += (m_iSelButtonSize + m_iMarginItem);
    }
}

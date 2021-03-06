#include "Texture.h"
#include "../Core/PathManager.h"
#include "../Application/Window.h"
#include "../Resources/ResourceManager.h"

Texture::Texture()
    :
    m_hMemDC(nullptr),
    m_bColorKeyEnable(false),
    m_ColorKey(RGB(255, 0, 255))
{

}

Texture::~Texture()
{
    SelectObject(m_hMemDC, m_hOldBitmap);
    DeleteObject(m_hBitmap);
    DeleteDC(m_hMemDC);
}

bool Texture::LoadTexture(HINSTANCE hInst, HDC hDC,
                          const string& strKey,
                          const wchar_t* pFileName,
                          const string& strPathKey)
{
    m_strFileName = pFileName;
    m_strKey = strKey;
    m_strPathKey = strPathKey;

    // 메모리 DC를 만들어준다.
    m_hMemDC = CreateCompatibleDC(hDC);

    // 전체 경로를 만들어준다.
    const wchar_t* pPath = PATH_MANAGER->FindPath(strPathKey);

    wstring strPath;
    if (pPath)
        strPath = pPath;

    strPath += pFileName;

    m_hBitmap = (HBITMAP)LoadImage(hInst, strPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
    GetObject(m_hBitmap, sizeof(m_tInfo), &m_tInfo);

    if (!m_hBitmap)
        return false;

    return true;
}

void Texture::DrawImageFrom(int px, int py, int sx, int sy, Texture* pTex, int u, int v)
{
    if (pTex->GetColorKeyEnable())
    {
        TransparentBlt(m_hMemDC, px, py, sx, sy,
            pTex->GetDC(), u, v,
            sx, sy,
            pTex->GetColorKey());
    }
    else
    {
        BitBlt(m_hMemDC, px, py, sx, sy, pTex->GetDC(), u, v, SRCCOPY);
    }
}

void Texture::DrawImageFrom(int px, int py, int sx, int sy, HDC orgin_hDC, int u, int v)
{
    BitBlt(m_hMemDC, px, py, sx, sy, orgin_hDC, u, v, SRCCOPY);
}

void Texture::TransparentEffect(HDC hdc, int px, int py, int sx, int sy, int u, int v)
{
    // 알파 블렌딩 버그 (y가 0 보다 작으면 아에 출력안됨 혹은 최대 클라이언트 y 영역 벗어나도)
    if (py < 0)
    {
        py = 0;
        sy += py;
    }
    else if (py + sy >= GETRESOLUTION.y)
    {
        sy -= (py + sy - GETRESOLUTION.y + 1);
    }
    Texture* pBack = RESOURCE_MANAGER->GetBackBuffer();
    Texture* pTemp = RESOURCE_MANAGER->GetTempBuffer();
    pTemp->ClearBuffer(px, py, sx, sy);
    pTemp->DrawImageFrom(px, py, sx, sy, pBack, px, py);
    pTemp->DrawImageFrom(px, py, sx, sy, this, u, v);


    AlphaBlend(hdc, px, py, sx, sy, pTemp->GetDC(), px, py, sx, sy, RESOURCE_MANAGER->GetTransparentFunc());
    SAFE_RELEASE(pTemp);
    SAFE_RELEASE(pBack);
}

void Texture::DrawImage(HDC hdc, int px, int py, int sx, int sy, int u, int v)
{
    if (GetColorKeyEnable())
    {
        TransparentBlt(hdc, px, py, sx, sy,
                        m_hMemDC, u, v,
                        sx, sy,
                        m_ColorKey);
    }
    else
    {
        BitBlt(hdc, px, py, sx, sy, m_hMemDC, u, v, SRCCOPY);
    }
}

Texture* Texture::CreateEmptyTexture(HDC hDC, int w, int h, COLORREF color)
{
    Texture* pTexture = new Texture;

    pTexture->m_hMemDC = CreateCompatibleDC(hDC);
    pTexture->m_hBitmap = CreateCompatibleBitmap(hDC, w, h);
    pTexture->m_hOldBitmap = (HBITMAP)SelectObject(pTexture->m_hMemDC, pTexture->m_hBitmap);
    DrawHDCWithColor(pTexture->m_hMemDC, w, h, color);

    return pTexture;
}

Texture* Texture::CreateCopyTexture(HDC hDC, int w, int h)
{
    Texture* pTexture = new Texture;

    pTexture->m_hMemDC = CreateCompatibleDC(hDC);
    pTexture->m_hBitmap = CreateCompatibleBitmap(hDC, w, h);
    pTexture->m_hOldBitmap = (HBITMAP)SelectObject(pTexture->m_hMemDC, pTexture->m_hBitmap);

    BitBlt(pTexture->m_hMemDC, 0, 0, w, h, hDC, 0, 0, SRCCOPY);

    return pTexture;
}


void Texture::SaveFromPath(const char* pFileName, const string& strPathKey)
{
}

void Texture::Save(FILE* pFile)
{
    int iLength = m_strKey.length();

    // key의 길이를 저장한다.
    fwrite(&iLength, 4, 1, pFile);
    fwrite(m_strKey.c_str(), 1, iLength, pFile);

    // FileName 저장
    iLength = m_strFileName.length();
    fwrite(&iLength, 4, 1, pFile);
    fwrite(m_strFileName.c_str(), 2, iLength, pFile);

    // PathKey 저장
    iLength = m_strPathKey.length();
    fwrite(&iLength, 4, 1, pFile);
    fwrite(m_strPathKey.c_str(), 1, iLength, pFile);

    // ColorKey
    fwrite(&m_bColorKeyEnable, 1, 1, pFile);
    fwrite(&m_ColorKey, sizeof(COLORREF), 1, pFile);
}

void Texture::LoadFromPath(const char* pFileName, const string& strPathKey)
{
}

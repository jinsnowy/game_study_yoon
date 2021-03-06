#pragma once
#include "../Object/Ref.h"
#include "../framework.h"
class Texture :
    public Ref
{
private:
    friend class ResourceManager;
    Texture();
    ~Texture();
public:
    bool LoadTexture(HINSTANCE hInst,
                     HDC hDC,
                     const string& strKey,
                     const wchar_t* pFileName,
                     const string& strPathKey = TEXTURE_PATH);
    void DrawImageAt(HDC hdc, const Pos& at)
    {
        DrawImageAt(hdc, int(at.x), int(at.y));
    }
    void DrawImageAt(HDC hdc, int px, int py)
    {
        Size tSize = GetSize();
        if (GetColorKeyEnable())
        {
            TransparentBlt(hdc, px, py, int(tSize.x), int(tSize.y),
                GetDC(), 0, 0,
                int(tSize.x), int(tSize.y),
                GetColorKey());
        }
        else
        {
            BitBlt(hdc, px, py, int(tSize.x), int(tSize.y),
                GetDC(), 0, 0, SRCCOPY);
        }
    }
    void DrawImageAtFixedSize(HDC hdc, const Pos& at, int size)
    {
        DrawImageAtFixedSize(hdc, int(at.x), int(at.y), size);
    }
    void DrawImageAtFixedSize(HDC hdc, int px, int py, int size)
    {
        Size tSize = GetSize();
        StretchBlt(hdc, px, py, size, size, GetDC(), 0, 0, int(tSize.x), int(tSize.y), SRCCOPY);
    }
    HDC GetDC() const { return m_hMemDC; }
    static Texture* CreateEmptyTexture(HDC hDC, int w, int h, COLORREF color = RGB(0,0,0));
    static Texture* CreateCopyTexture(HDC hDC, int w, int h);
private:
    HDC         m_hMemDC;
    HBITMAP     m_hBitmap;
    HBITMAP     m_hOldBitmap;
    BITMAP      m_tInfo;
    COLORREF    m_ColorKey;
    bool        m_bColorKeyEnable;
    wstring     m_strFileName;
    string      m_strKey;
    string      m_strPathKey;
public:
    string GetTag() const override
    {
        return m_strKey;
    }
    void SetColorKey(unsigned char r, unsigned char g, unsigned char b)
    {
        m_bColorKeyEnable = true;
        m_ColorKey = RGB(r, g, b);
    }
    void SetColorKey(const COLORREF& key)
    {
        m_bColorKeyEnable = true;
        m_ColorKey = key;
    }
    COLORREF GetColorKey() const
    {
        return m_ColorKey;
    }
    bool GetColorKeyEnable() const
    {
        return m_bColorKeyEnable;
    }
    Size GetSize() const { return { float(m_tInfo.bmWidth), float(m_tInfo.bmHeight) }; }
    long GetWidth() const
    {
        return m_tInfo.bmWidth;
    }
    long GetHeight() const
    {
        return m_tInfo.bmHeight;
    }
    void SaveFromPath(const char* pFileName, const string& strPathKey);
    void Save(FILE* pFile);
    void LoadFromPath(const char* pFileName, const string& strPathKey);
};
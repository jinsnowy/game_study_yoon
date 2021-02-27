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
    HDC GetDC() const { return m_hMemDC; }
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
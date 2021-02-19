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
                    const char* pFileName,
                    const string& strPathKey = TEXTURE_PATH);
    HDC GetDC() const { return m_hMemDC; }
private:
    HDC     m_hMemDC;
    HBITMAP m_hBitmap;
    HBITMAP m_hOldBitmap;
    BITMAP  m_tInfo;
};


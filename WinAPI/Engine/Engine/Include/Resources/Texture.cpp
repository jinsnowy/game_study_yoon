#include "Texture.h"
#include "../Core/PathManager.h"
Texture::Texture()
    :
    m_hMemDC(nullptr),
    m_bColorKeyEnable(false),
    m_ColorKey(RGB(255, 0, 255))
{
}

Texture::~Texture()
{
    // 기존에 지정되어 있던 도구를 다시 지정해준다.
    SelectObject(m_hMemDC, m_hOldBitmap);

    // Bitmap을 지워준다.
    DeleteObject(m_hBitmap);

    // DC를 지워준다.
    DeleteDC(m_hMemDC);
}

bool Texture::LoadTexture(HINSTANCE hInst, HDC hDC,
                          const string& strKey, const wchar_t* pFileName, const string& strPathKey)
{
    // 메모리 DC를 만들어준다.
    m_hMemDC = CreateCompatibleDC(hDC);

    // 전체 경로를 만들어준다.
    const wchar_t* pPath = PATH_MANAGER.FindPath(strPathKey);

    wstring strPath;
    if (pPath)
        strPath = pPath;

    strPath += pFileName;

    // 전체 경로를 만들어준다.
    m_hBitmap = (HBITMAP)LoadImage(hInst, strPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // 위에서 만들어준 비트맵 도구를 DC에 지정한다.
    // 지정할때 반환되는 값을 DC에 기본으로 지정되어 있던 도구가 반환된다.
    m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

    // 비트맵 정보를 가져온다.
    GetObject(m_hBitmap, sizeof(m_tInfo), &m_tInfo);

    return true;
}

#include "Texture.h"
#include "../Core/PathManager.h"
Texture::Texture()
    :
    m_hMemDC(nullptr)
{
}

Texture::~Texture()
{
    // ������ �����Ǿ� �ִ� ������ �ٽ� �������ش�.
    SelectObject(m_hMemDC, m_hOldBitmap);

    // Bitmap�� �����ش�.
    DeleteObject(m_hBitmap);

    // DC�� �����ش�.
    DeleteDC(m_hMemDC);
}

bool Texture::LoadTexture(HINSTANCE hInst, HDC hDC,
                          const string& strKey, const char* pFileName, const string& strPathKey)
{
    // �޸� DC�� ������ش�.
    m_hMemDC = CreateCompatibleDC(hDC);

    // ��ü ��θ� ������ش�.
    const char* pPath = PATH_MANAGER.FindPath(strPathKey);

    string strPath;
    if (pPath)
        strPath = pPath;

    strPath += pFileName;

    // ��ü ��θ� ������ش�.
    m_hBitmap = (HBITMAP)LoadImage(hInst, strPath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // ������ ������� ��Ʈ�� ������ DC�� �����Ѵ�.
    // �����Ҷ� ��ȯ�Ǵ� ���� DC�� �⺻���� �����Ǿ� �ִ� ������ ��ȯ�ȴ�.
    m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

    // ��Ʈ�� ������ �����´�.
    GetObject(m_hBitmap, sizeof(m_tInfo), &m_tInfo);

    return true;
}
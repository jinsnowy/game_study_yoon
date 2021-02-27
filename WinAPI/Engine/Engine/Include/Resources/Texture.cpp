#include "Texture.h"
#include "../Core/PathManager.h"
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
    // ������ �����Ǿ� �ִ� ������ �ٽ� �������ش�.
    SelectObject(m_hMemDC, m_hOldBitmap);

    // Bitmap�� �����ش�.
    DeleteObject(m_hBitmap);

    // DC�� �����ش�.
    DeleteDC(m_hMemDC);
}

bool Texture::LoadTexture(HINSTANCE hInst, HDC hDC,
                          const string& strKey, const wchar_t* pFileName, const string& strPathKey)
{
    m_strFileName = pFileName;
    m_strKey = strKey;
    m_strPathKey = strPathKey;

    // �޸� DC�� ������ش�.
    m_hMemDC = CreateCompatibleDC(hDC);

    // ��ü ��θ� ������ش�.
    const wchar_t* pPath = PATH_MANAGER->FindPath(strPathKey);

    wstring strPath;
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

void Texture::SaveFromPath(const char* pFileName, const string& strPathKey)
{
}

void Texture::Save(FILE* pFile)
{
    int iLength = m_strKey.length();

    // key�� ���̸� �����Ѵ�.
    fwrite(&iLength, 4, 1, pFile);
    fwrite(m_strKey.c_str(), 1, iLength, pFile);

    // FileName ����
    iLength = m_strFileName.length();
    fwrite(&iLength, 4, 1, pFile);
    fwrite(m_strFileName.c_str(), 2, iLength, pFile);

    // PathKey ����
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

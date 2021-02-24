#include "ColliderPixel.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "../Core/PathManager.h"

ColliderPixel::ColliderPixel()
{
    m_eCollType = CT_PIXEL;
    m_iWidth = 0;
    m_iHeight = 0;
}

ColliderPixel::ColliderPixel(const ColliderPixel& coll)
    :
    Collider(coll)
{
    m_iWidth = coll.m_iWidth;
    m_iHeight = coll.m_iHeight;
}

ColliderPixel::~ColliderPixel()
{
}

bool ColliderPixel::SetPixelInfo(const char* pFileName, const string& strPathKey)
{
    const char* pPath = PATH_MANAGER.FindPathByMultiByte(strPathKey);
    string strPath;

    if (pPath)
        strPath = pPath;

    strPath += pFileName;

    FILE* pFile = NULL;

    fopen_s(&pFile, strPath.c_str(), "rb");

    if (!pFile)
        return false;

    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;

    fread(&fh, sizeof(fh), 1, pFile);
    fread(&ih, sizeof(ih), 1, pFile);

    m_iWidth = ih.biWidth;
    m_iHeight = ih.biHeight;

    m_vecPixel.clear();

    m_vecPixel.resize(m_iWidth * m_iHeight);

    fread(&m_vecPixel[0], sizeof(Pixel), m_vecPixel.size(), pFile);

    // �� �Ʒ��� ���������ش�.
    Pixel* pPixelArr = new Pixel[m_iWidth];
    for (int i = 0; i < m_iHeight / 2; ++i)
    {
        // ���� �ε����� �ȼ� �� ���� �����صд�.
        memcpy(pPixelArr, &m_vecPixel[i * m_iWidth], sizeof(Pixel) * m_iWidth);
        memcpy(&m_vecPixel[i * m_iWidth], &m_vecPixel[(m_iHeight - 1 - i) * m_iWidth], sizeof(Pixel) * m_iWidth);
        memcpy(&m_vecPixel[(m_iHeight - 1 - i) * m_iWidth], pPixelArr, sizeof(Pixel) * m_iWidth);
    }

    delete[] pPixelArr;

    fclose(pFile);

    /*fopen_s(&pFile, "Test.bmp", "wb");

    fwrite(&fh, sizeof(fh), 1, pFile);
    fwrite(&ih, sizeof(ih), 1, pFile);
    fwrite(&m_vecPixel[0], sizeof(Pixel), m_vecPixel.size(), pFile);

    fclose(pFile);*/

    return true;
}

bool ColliderPixel::Init()
{
    return true;
}

void ColliderPixel::Input(float dt)
{
}

int ColliderPixel::Update(float dt)
{
    return 0;
}

int ColliderPixel::LateUpdate(float dt)
{
    return 0;
}

bool ColliderPixel::CheckCollision(Collider* pDst)
{
    switch (pDst->GetColliderType())
    {
    case CT_RECT:
        return CollisionRectToPixel(static_cast<ColliderRect*>(pDst)->GetWorldInfo(), m_vecPixel, m_iWidth, m_iHeight);
    }
    return true;
}

void ColliderPixel::Draw(HDC hdc, float dt)
{
}

ColliderPixel* ColliderPixel::Clone()
{
    return new ColliderPixel(*this);
}
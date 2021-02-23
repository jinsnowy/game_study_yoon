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

bool ColliderPixel::SetPixelInfo(char* pFileName, const string& strPathKey)
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

    fclose(pFile);

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
    return true;
}

void ColliderPixel::Draw(HDC hdc, float dt)
{
}

ColliderPixel* ColliderPixel::Clone()
{
    return new ColliderPixel(*this);
}

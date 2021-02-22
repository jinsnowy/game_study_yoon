#include "ColliderPixel.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "../Core/PathManager.h"

ColliderPixel::ColliderPixel()
{
    m_eCollType = CT_RECT;
}

ColliderPixel::ColliderPixel(const ColliderPixel& coll)
    :
    Collider(coll)
{
}

ColliderPixel::~ColliderPixel()
{
}

bool ColliderPixel::SetPixelInfo(char* pFileName, const string& strPathKey)
{
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

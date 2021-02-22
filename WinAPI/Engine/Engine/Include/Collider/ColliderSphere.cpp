#include "ColliderSphere.h"
#include "ColliderRect.h"
#include "../Object/Object.h"

ColliderSphere::ColliderSphere()
{
    m_eCollType = CT_SPHERE;
}

ColliderSphere::ColliderSphere(const ColliderSphere& coll)
    :
    Collider(coll)
{
    m_tInfo = coll.m_tInfo;
    m_tWorldInfo = coll.m_tWorldInfo;
}

ColliderSphere::~ColliderSphere()
{
}

bool ColliderSphere::Init()
{
    return true;
}

void ColliderSphere::Input(float dt)
{
    Collider::Input(dt);
}

int ColliderSphere::Update(float dt)
{
    Collider::Update(dt);
    return 0;
}

int ColliderSphere::LateUpdate(float dt)
{
    Collider::LateUpdate(dt);

    Pos tPos = m_pObject->GetPos();
    m_tWorldInfo.center = tPos + m_tInfo.center;
    m_tWorldInfo.radius = m_tInfo.radius;

    return 0;
}

bool ColliderSphere::CheckCollision(Collider* pDst)
{
    switch (pDst->GetColliderType())
    {
    case CT_SPHERE:
        return CollisionSphereToSphere(m_tWorldInfo, ((ColliderSphere*)pDst)->GetWorldInfo());
    case CT_RECT:
        return CollisionRectToSphere(((ColliderRect*)pDst)->GetWorldInfo(), m_tWorldInfo);
    }
    return false;
}

void ColliderSphere::Draw(HDC hdc, float dt)
{
    Collider::Draw(hdc,dt);
}

ColliderSphere* ColliderSphere::Clone()
{
    return new ColliderSphere(*this);
}

#include "ColliderSphere.h"
#include "ColliderRect.h"
#include "ColliderPixel.h"
#include "../Object/Object.h"
#include "../Core/Camera.h"

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

    HPEN myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    HPEN OldPen = (HPEN)SelectObject(hdc, myPen);

    HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Pos tPos = CAMERA.GetTopLeft();

    int left = m_tWorldInfo.center.x - m_tWorldInfo.radius - tPos.x;
    int right = left + 2 * m_tWorldInfo.radius;
    int top = m_tWorldInfo.center.y - m_tWorldInfo.radius - tPos.y;
    int bottom = top + 2 * m_tWorldInfo.radius;

    Ellipse(hdc, left, top, right, bottom);

    DeleteObject(SelectObject(hdc, OldPen));
    DeleteObject(SelectObject(hdc, OldBrush));
}

ColliderSphere* ColliderSphere::Clone()
{
    return new ColliderSphere(*this);
}

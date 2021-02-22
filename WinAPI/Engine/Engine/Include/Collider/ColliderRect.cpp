#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"
#include "../Object/Object.h"

ColliderRect::ColliderRect()
	:
	Collider(),
	m_tInfo(),
	m_tWorldInfo()
{
}

ColliderRect::ColliderRect(const ColliderRect& coll)
	:
	Collider(coll),
	m_tInfo(coll.m_tInfo),
	m_tWorldInfo(coll.m_tWorldInfo)
{
}

ColliderRect::~ColliderRect()
{
}

void ColliderRect::SetRect(float l, float t, float r, float b)
{
	m_tInfo.SetRect(l, t, r, b);
}

bool ColliderRect::Init()
{
	return true;
}

void ColliderRect::Input(float dt)
{
	Collider::Input(dt);
}

int ColliderRect::Update(float dt)
{
	Collider::Update(dt);
	return 0;
}

int ColliderRect::LateUpdate(float dt)
{
	Collider::LateUpdate(dt);
	
	Pos tPos = m_pObject->GetPos();
	m_tWorldInfo.left = tPos.x + m_tInfo.left;
	m_tWorldInfo.top = tPos.y + m_tInfo.top;
	m_tWorldInfo.right = tPos.x + m_tInfo.right;
	m_tWorldInfo.bottom = tPos.y + m_tInfo.bottom;

	return 0;
}

bool ColliderRect::CheckCollision(Collider* pDst)
{
	switch (pDst->GetColliderType())
	{
	case CT_RECT:
		return CollisionRectToRect(m_tWorldInfo, ((ColliderRect*)pDst)->GetWorldInfo());
	case CT_SPHERE:
		return  CollisionRectToSphere(m_tWorldInfo, ((ColliderSphere*)pDst)->GetWorldInfo());
	}
	return false;
}

void ColliderRect::Draw(HDC hDC, float dt)
{
	Collider::Draw(hDC, dt);
}

ColliderRect* ColliderRect::Clone()
{
	return new ColliderRect(*this);
}

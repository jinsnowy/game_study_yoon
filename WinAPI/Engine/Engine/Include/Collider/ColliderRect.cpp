#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "ColliderPixel.h"
#include "ColliderPoint.h"
#include "../Object/Object.h"
#include "../Core/Camera.h"

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
		return CollisionRectToRect(m_tWorldInfo, static_cast<ColliderRect*>(pDst)->GetWorldInfo());
	case CT_SPHERE:
		return CollisionRectToSphere(m_tWorldInfo, static_cast<ColliderSphere*>(pDst)->GetWorldInfo());
	case CT_PIXEL:
		return CollisionRectToPixel(m_tWorldInfo, static_cast<ColliderPixel*>(pDst)->GetPixel(),
											      static_cast<ColliderPixel*>(pDst)->GetWidth(), 
											      static_cast<ColliderPixel*>(pDst)->GetHeight());
	case CT_POINT:
		return CollisionRectToPoint(m_tWorldInfo, static_cast<ColliderPoint*>(pDst)->GetWorldInfo());
	}
	return false;
}

void ColliderRect::Draw(HDC hDC, float dt)
{
#ifdef _DEBUG
	Collider::Draw(hDC, dt);

	Pos tPos = Pos(0, 0);
	if (!m_bUIColl)
		tPos = CAMERA->GetTopLeft();

	int left = int(m_tWorldInfo.left - tPos.x);
	int top = int(m_tWorldInfo.top - tPos.y);
	int right = int(m_tWorldInfo.right - tPos.x);
	int bottom = int(m_tWorldInfo.bottom - tPos.y);

	HPEN myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	HPEN OldPen = (HPEN)SelectObject(hDC, myPen);
	HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

	Rectangle(hDC, left, top, right, bottom);
	
	DeleteObject(SelectObject(hDC, OldPen));
	DeleteObject(SelectObject(hDC, OldBrush));
#endif
}

void ColliderRect::Save(FILE* pFile)
{
	Collider::Save(pFile);
}

void ColliderRect::Load(FILE* pFile)
{
	Collider::Load(pFile);
}

ColliderRect* ColliderRect::Clone()
{
	return new ColliderRect(*this);
}

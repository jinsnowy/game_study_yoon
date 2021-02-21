#include "Collider.h"
#include "../Object/Object.h"

Collider::Collider()
	:
	m_eCollType(CT_RECT)
{
}

Collider::Collider(const Collider& coll)
{
	*this = coll;
}

Collider::~Collider()
{
	// 날 충돌체로 판별하여 리스트로 가진 녀석들을 없애달라고 요청해야함
	list<Collider*>::iterator iter;
	list<Collider*>::iterator iterEnd = m_CollisionList.end();

	for (iter = m_CollisionList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->EraseCollisionList(this);
	}
}

void Collider::Input(float dt)
{
}

int Collider::Update(float dt)
{
	return 0;
}

int Collider::LateUpdate(float dt)
{ 
	return 0;
}

bool Collider::CheckCollision(Collider* pDst)
{
	return true;
}


void Collider::Draw(HDC hdc, float dt)
{
}

bool Collider::CollisionRectToRect(const Rect& src, const Rect& dst)
{
	if (src.right < dst.left) return false;
	if (src.left > dst.right) return false;
	if (src.bottom < dst.top) return false;
	if (src.top > dst.bottom) return false;
	return true;
}

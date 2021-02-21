#include "ColliderRect.h"

ColliderRect::ColliderRect()
{
}

ColliderRect::ColliderRect(const ColliderRect& coll)
	:
	Collider(coll)
{
}

ColliderRect::~ColliderRect()
{
}

bool ColliderRect::Init()
{
	return true;
}

void ColliderRect::Input(float dt)
{
}

int ColliderRect::Update(float dt)
{
	return 0;
}

int ColliderRect::LateUpdate(float dt)
{
	return 0;
}

void ColliderRect::Collision(float dt)
{
}

void ColliderRect::Draw(HDC hdc, float dt)
{
}

ColliderRect* ColliderRect::Clone()
{
	return new ColliderRect(*this);
}

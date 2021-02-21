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

void Collider::Collision(float dt)
{
}

void Collider::Draw(HDC hdc, float dt)
{
}

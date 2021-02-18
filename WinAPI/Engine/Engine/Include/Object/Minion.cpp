#include "Minion.h"
#include "../framework.h"

Minion::Minion()
{
	m_Dir = MD_FRONT;
}

Minion::Minion(const Minion& obj)
	: MovableObject(obj), m_Dir(obj.m_Dir)
{
}

Minion::~Minion()
{
}

bool Minion::Init()
{
	SetPos(800.0f, 100.0f);
	SetSize(100.0f, 100.0f);
	SetSpeed(300.0f);

	return true;
}

void Minion::Input(float dt)
{
	MovableObject::Input(dt);
	MovableObject::Input(dt);

	MoveYFromSpeed(dt, m_Dir);

	
	if (m_Pos.y + m_Size.y >= GETRESOLUTION.iH)
	{
		m_Pos.y = GETRESOLUTION.iH - m_Size.y;
		m_Dir = MD_BACK;
	}

	else if (m_Pos.y <= 0.f)
	{
		m_Pos.y = 0.f;
		m_Dir = MD_FRONT;
	}

}

int Minion::Update(float dt)
{
	MovableObject::Update(dt);
	return 0;
}

int Minion::LateUpdate(float dt)
{
	MovableObject::LateUpdate(dt);
	return 0;
}

void Minion::Collision(float dt)
{
	MovableObject::Collision(dt);
}

void Minion::Render(HDC hDC, float dt)
{
	MovableObject::Render(hDC, dt);
	Rectangle(hDC, (int)m_Pos.x, (int)m_Pos.y, int(m_Pos.x + m_Size.x), int(m_Pos.y + m_Size.y));
}

#include "Minion.h"
#include "../framework.h"
#include "../Application/Window.h"

Minion::Minion()
	:
	m_FireTime(0.f),
	m_FireLimitTime(1.f),
	m_Dir(MD_FRONT)
{
}

Minion::Minion(const Minion& obj)
	: MovableObject(obj), 
	m_Dir(obj.m_Dir),
	m_FireTime(obj.m_FireTime),
	m_FireLimitTime(obj.m_FireLimitTime)
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

	if (m_Pos.y + m_Size.y >= GETRESOLUTION.y)
	{
		m_Pos.y = GETRESOLUTION.y - m_Size.y;
		m_Dir = MD_BACK;
	}

	else if (m_Pos.y <= 0.f)
	{
		m_Pos.y = 0.f;
		m_Dir = MD_FRONT;
	}

	m_FireTime += dt;

	if (m_FireTime >= m_FireLimitTime)
	{
		m_FireTime -= m_FireLimitTime;
		Fire();
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

void Minion::Draw(HDC hDC, float dt)
{
	MovableObject::Draw(hDC, dt);
	Rectangle(hDC, (int)m_Pos.x, (int)m_Pos.y, int(m_Pos.x + m_Size.x), int(m_Pos.y + m_Size.y));
}

Minion* Minion::Clone()
{
	return new Minion(*this);
}

void Minion::Fire()
{
	Object* pBullet = Object::CreateCloneObject("Bullet", "MinionBullet", m_pLayer);

	((MovableObject*)pBullet)->SetAngle(PI);

	pBullet->SetPos(m_Pos.x - pBullet->GetSize().x, (m_Pos.y + m_Pos.y + m_Size.y) / 2.f - pBullet->GetSize().y / 2.f);

	SAFE_RELEASE(pBullet);
}
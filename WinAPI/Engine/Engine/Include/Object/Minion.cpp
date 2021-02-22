#include "Minion.h"
#include "../framework.h"
#include "../Application/Window.h"
#include "../Resources/Texture.h"
#include "../Collider/ColliderRect.h"
#include "Bullet.h"

Minion::Minion()
	:
	m_FireTime(0.f),
	m_FireLimitTime(0.3f),
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
	SetPivot(0.5f, 0.5f);
	SetSpeed(300.0f);
	m_Dir = MD_FRONT;

	SetTexture("Minion", "minion.bmp");
	m_pTexture->SetColorKey(255, 0, 255);

	ColliderRect* pRC = AddCollider<ColliderRect>("MinionBody");

	pRC->SetRect(-50.f, -50.f, 50.f, 50.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &Minion::CollisionBullet);

	SAFE_RELEASE(pRC);

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
}

Minion* Minion::Clone()
{
	return new Minion(*this);
}

void Minion::CollisionBullet(Collider* pSrc, Collider* pDst, float dt)
{
	// Die();
	// MessageBox(NULL, "�浹", "�浹", MB_OK);
}

void Minion::Fire()
{
	Object* pBullet = Object::CreateCloneObject("Bullet", "MinionBullet", m_pLayer);

	pBullet->AddCollisionFunction("BulletBody", CS_ENTER, (Bullet*)pBullet, &Bullet::Hit);

	// ((MovableObject*)pBullet)->SetSpeed(100.0f);
	((MovableObject*)pBullet)->SetAngle(PI);

	float x = GetLeft() - (1.f - pBullet->GetPivot().x) * pBullet->GetSize().x;
	float y = GetCenter().y;

	pBullet->SetPos(x, y);

	SAFE_RELEASE(pBullet);
}
#include "Player.h"
#include "Bullet.h"
#include "../Core/Input.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Camera.h"

Player::Player()
{
}

Player::Player(const Player& obj) : MovableObject(obj)
{
}

Player::~Player()
{
}

bool Player::Init()
{
	SetPos(50.0f, 50.0f);
	SetSize(100.0f, 100.0f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(400.0f);

	SetTexture("Player", L"hos.bmp");

	m_iHP = 1000;

	SetPhysics(false);

	ColliderRect* pRC = AddCollider<ColliderRect>("PlayerBody");

	pRC->SetRect(-50.f, -50.f, 50.f, 50.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &Player::Hit);

	SAFE_RELEASE(pRC);

	return true;
}

void Player::Input(float dt)
{
	MovableObject::Input(dt);
	if (KEYPRESS("MoveBack"))
	{
		MoveYFromSpeed(dt, MD_FRONT);
	}
	if (KEYPRESS("MoveFront"))
	{
		MoveYFromSpeed(dt, MD_BACK);
	}
	if (KEYPRESS("MoveLeft"))
	{
		MoveXFromSpeed(dt, MD_BACK);
	}
	if (KEYPRESS("MoveRight"))
	{
		MoveXFromSpeed(dt, MD_FRONT);
	}
	if (KEYDOWN("Fire"))
	{
		Fire();
	}
	if (KEYDOWN("Skill1"))
	{
		MessageBox(NULL, L"Skill1", L"Skill1", MB_OK);
	}
}

int Player::Update(float dt)
{
	MovableObject::Update(dt);
	return 0;
}

int Player::LateUpdate(float dt)
{
	MovableObject::LateUpdate(dt);
	return 0;
}

void Player::Collision(float dt)
{
	MovableObject::Collision(dt);
}

void Player::Draw(HDC hDC, float dt)
{
  	MovableObject::Draw(hDC, dt);
	wchar_t strHP[32] = {};
	swprintf_s(strHP, L"HP: %d", m_iHP);
	Pos tPos = m_Pos - m_Size * m_Pivot;
	tPos -= CAMERA.GetTopLeft();
	TextOut(hDC, tPos.x, tPos.y, strHP, lstrlen(strHP));
}

Player* Player::Clone()
{
	return new Player(*this);
}

void Player::Hit(Collider* pSrc, Collider* pDst, float dt)
{
	m_iHP -= 5;
}

void Player::Fire()
{
	Object* pBullet = Object::CreateCloneObject("Bullet", "PlayerBullet", m_pLayer);

	pBullet->AddCollisionFunction("BulletBody", CS_ENTER, (Bullet*) pBullet, &Bullet::Hit);

	Pos tPos;
	tPos.x = GetRight() + pBullet->GetPivot().x * pBullet->GetSize().x;
	tPos.y = GetCenter().y;

	pBullet->SetPos(tPos.x, tPos.y - pBullet->GetSize().y / 2.f);

	SAFE_RELEASE(pBullet);
}

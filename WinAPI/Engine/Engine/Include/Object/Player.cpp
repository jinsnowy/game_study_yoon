#include "Player.h"
#include "../Core/Input.h"
#include "../Collider/ColliderRect.h"
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

	SetTexture("Player", "hos.bmp");

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
		MessageBox(NULL, "Skill1", "Skill1", MB_OK);
	}
}

int  Player::Update(float dt)
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
	// Rect(hDC, (int)m_Pos.x, (int)m_Pos.y, int(m_Pos.x + m_Size.x), int(m_Pos.y + m_Size.y));
}

Player* Player::Clone()
{
	return new Player(*this);
}

void Player::Fire()
{
	Object* pBullet = Object::CreateCloneObject("Bullet", "PlayerBullet", m_pLayer);

	// ������ ����� ���Ѵ�.
	Pos tPos;
	tPos.x = GetRight() + pBullet->GetPivot().x * pBullet->GetSize().x;
	tPos.y = GetCenter().y;

	pBullet->SetPos(tPos.x, tPos.y - pBullet->GetSize().y / 2.f);

	SAFE_RELEASE(pBullet);
}

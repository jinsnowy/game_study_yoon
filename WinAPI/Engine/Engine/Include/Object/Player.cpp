#include "Player.h"

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

	if (GetAsyncKeyState('W') & 0x8000)
	{
		MoveYFromSpeed(dt, MD_BACK);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		MoveYFromSpeed(dt, MD_FRONT);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		MoveXFromSpeed(dt, MD_BACK);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		MoveXFromSpeed(dt, MD_FRONT);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		Fire();
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
	// Rectangle(hDC, (int)m_Pos.x, (int)m_Pos.y, int(m_Pos.x + m_Size.x), int(m_Pos.y + m_Size.y));
}

Player* Player::Clone()
{
	return new Player(*this);
}

void Player::Fire()
{
	Object* pBullet = Object::CreateCloneObject("Bullet", "PlayerBullet", m_pLayer);

	// 오른쪽 가운데를 구한다.
	Pos tPos;
	tPos.x = GetRight() + pBullet->GetPivot().x * pBullet->GetSize().x;
	tPos.y = GetCenter().y;

	pBullet->SetPos(tPos.x, tPos.y - pBullet->GetSize().y / 2.f);

	SAFE_RELEASE(pBullet);
}

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
	SetPos(Pos(100.0f, 100.0f));
	SetSize(Size(300.0f, 300.0f));
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

	pBullet->SetPos(m_Pos.x + m_Size.x, (m_Pos.y + m_Pos.y + m_Size.y) / 2.f - pBullet->GetSize().y / 2.f);

	SAFE_RELEASE(pBullet);
}

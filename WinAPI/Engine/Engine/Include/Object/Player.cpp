#include "Player.h"

Player::Player()
{
}

Player::Player(const Player& obj)
	: MovableObject(obj)
{
}

Player::~Player()
{
}

bool Player::Init()
{
	SetPos(Pos(100.0f, 100.0f));
	SetSize(Size(100.0f, 100.0f));
	SetSpeed(400.0f);

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

void Player::Render(HDC hDC, float dt)
{
	MovableObject::Render(hDC, dt);
	Rectangle(hDC, (int)m_Pos.x, (int)m_Pos.y, (int)m_Pos.x + m_Size.x, (int)m_Pos.y + m_Size.y);
}
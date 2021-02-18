#include "MovableObject.h"

MovableObject::MovableObject()
{
}

MovableObject::MovableObject(const MovableObject& obj) : Object(obj)
{
}

MovableObject::~MovableObject()
{
}

void MovableObject::Input(float dt)
{
}

int MovableObject::Update(float dt)
{
    return 0;
}

int MovableObject::LateUpdate(float dt)
{
    return 0;
}

void MovableObject::Collision(float dt)
{
}

void MovableObject::Render(HDC hDC, float dt)
{
}

void MovableObject::Move(float x, float y)
{
	m_Pos.x += x;
	m_Pos.y += y;
}

void MovableObject::Move(float x, float y, float dt)
{
	m_Pos.x += x * dt;
	m_Pos.y += y * dt;
}

void MovableObject::Move(const Pos& tMove, float dt)
{
	m_Pos += tMove * dt;
}

void MovableObject::MoveXFromSpeed(float dt, MOVE_DIR dir)
{
	m_Pos.x += m_Speed * dt *  float(dir);
}

void MovableObject::MoveYFromSpeed(float dt, MOVE_DIR dir)
{
	m_Pos.y += m_Speed * dt * float(dir);
}

void MovableObject::MoveX(float x)
{
	m_Pos.x += x;
}

void MovableObject::MoveX(float x, float dt)
{
	m_Pos.x += x * dt;
}

void MovableObject::MoveY(float y)
{
	m_Pos.y += y;
}

void MovableObject::MoveY(float y, float dt)
{
	m_Pos.y += y * dt;
}

void MovableObject::MoveAngle(float fSpeed)
{
	m_Pos.x += cosf(m_Angle) * fSpeed;
	m_Pos.y += sinf(m_Angle) * fSpeed;
}

void MovableObject::MoveAngle(float fSpeed, float fTime)
{
	m_Pos.x += cosf(m_Angle) * fSpeed * fTime;
	m_Pos.y += sinf(m_Angle) * fSpeed * fTime;
}

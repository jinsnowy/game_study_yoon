#include "MovableObject.h"

MovableObject::MovableObject() 
	: m_Angle(0.f), m_Speed(100.f)
{
}

MovableObject::MovableObject(const MovableObject& obj) 
	: Object(obj),
	m_Angle(obj.m_Angle),
	m_Speed(obj.m_Speed)
{
}

MovableObject::~MovableObject()
{
}

bool MovableObject::Init()
{
	return true;
}

void MovableObject::Input(float dt)
{
	Object::Input(dt);
}

int MovableObject::Update(float dt)
{
	Object::Update(dt);
    return 0;
}

int MovableObject::LateUpdate(float dt)
{
	Object::LateUpdate(dt);
    return 0;
}

void MovableObject::Collision(float dt)
{
	Object::Collision(dt);
}

void MovableObject::Draw(HDC hDC, float dt)
{
	Object::Draw(hDC, dt);
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

void MovableObject::MoveAngle()
{
	m_Pos.x += cosf(m_Angle) * m_Speed;
	m_Pos.y += sinf(m_Angle) * m_Speed;
}

void MovableObject::MoveAngle(float fTime)
{
	m_Pos.x += cosf(m_Angle) * m_Speed * fTime;
	m_Pos.y += sinf(m_Angle) * m_Speed * fTime;
}

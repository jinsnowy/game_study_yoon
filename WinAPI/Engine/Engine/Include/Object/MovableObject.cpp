#include "MovableObject.h"

MovableObject::MovableObject() 
	: m_Angle(0.f), m_Speed(100.f), m_bMove(false), m_bFalling(false),
	m_fForce(0.f), m_fForceOrigin(0.f)
{
}

MovableObject::MovableObject(const MovableObject& obj) 
	: Object(obj),
	m_Angle(obj.m_Angle),
	m_Speed(obj.m_Speed)
{

	m_fForceOrigin = obj.m_fForceOrigin;
	m_fForce = 0.f;

	m_bMove = false;
	m_bFalling = false;
}

MovableObject::~MovableObject()
{
}

void MovableObject::Jump()
{
	if (!m_bFalling)
	{
		m_bFalling = true;
		m_fForce = m_fForceOrigin;
	}
}

void MovableObject::JumpEnd()
{
	m_bFalling = false;
	m_fForce = 0.f;
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
	if (m_blsPhysics)
	{
		//m_fGravityTime += dt;

		//// 점프 상태일 경우 힘을 감소시킨다.
		//m_fForce -= (GRAVITY * m_fGravityTime * m_fGravityTime);
		//m_Pos.y  -= m_fForce * dt;

		m_fForce -= GRAVITY * dt;
		m_Pos.y  -= m_fForce * dt - 0.5f * GRAVITY * dt * dt;
	}

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
	m_bMove = false;
}

void MovableObject::Move(float x, float y)
{
	m_Pos.x += x;
	m_Pos.y += y;

	m_bMove = true;
}

void MovableObject::Move(float x, float y, float dt)
{
	m_Pos.x += x * dt;
	m_Pos.y += y * dt;

	m_bMove = true;
}

void MovableObject::Move(const Pos& tMove, float dt)
{
	m_Pos += tMove * dt;

	m_bMove = true;
}

void MovableObject::MoveXFromSpeed(float dt, MOVE_DIR dir)
{
	m_Pos.x += m_Speed * dt *  float(dir);

	m_bMove = true;
}

void MovableObject::MoveYFromSpeed(float dt, MOVE_DIR dir)
{
	m_Pos.y += m_Speed * dt * float(dir);

	m_bMove = true;
}

void MovableObject::MoveX(float x)
{
	m_Pos.x += x;

	m_bMove = true;
}

void MovableObject::MoveX(float x, float dt)
{
	m_Pos.x += x * dt;

	m_bMove = true;
}

void MovableObject::MoveY(float y)
{
	m_Pos.y += y;

	m_bMove = true;
}

void MovableObject::MoveY(float y, float dt)
{
	m_Pos.y += y * dt;

	m_bMove = true;
}

void MovableObject::MoveAngle()
{
	m_Pos.x += cosf(m_Angle) * m_Speed;
	m_Pos.y += sinf(m_Angle) * m_Speed;

	m_bMove = true;
}

void MovableObject::MoveAngle(float fTime)
{
	m_Pos.x += cosf(m_Angle) * m_Speed * fTime;
	m_Pos.y += sinf(m_Angle) * m_Speed * fTime;

	m_bMove = true;
}

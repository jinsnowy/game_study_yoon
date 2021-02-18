#include "CMoveObj.h"

CMoveObj::CMoveObj()
	: m_fAngle(0.f),
	  m_fSpeed(100.0f)
{
}

CMoveObj::CMoveObj(const CMoveObj& obj)
	: CObject(obj),
	m_fAngle(obj.m_fAngle),
	m_fSpeed(obj.m_fSpeed)
{
	
}

CMoveObj::~CMoveObj()
{
}

void CMoveObj::Move(float x, float y)
{
	m_tPos.x += x;
	m_tPos.y += y;
}

void CMoveObj::Move(float x, float y, float fDeltaTime)
{
	m_tPos.x += x * fDeltaTime;
	m_tPos.y += y * fDeltaTime;
}

void CMoveObj::Move(const POSITION& tMove, float fDeltaTime)
{
	m_tPos += tMove * fDeltaTime;
}

void CMoveObj::MoveXFromSpeed(float fDeltaTime, MOVE_DIR dir)
{
	m_tPos.x += m_fSpeed * fDeltaTime * dir;
}

void CMoveObj::MoveYFromSpeed(float fDeltaTime, MOVE_DIR dir)
{
	m_tPos.y += m_fSpeed * fDeltaTime * dir;
}


void CMoveObj::MoveX(float x)
{
	m_tPos.x += x;
}

void CMoveObj::MoveX(float x, float fDeltaTime)
{
	m_tPos.x += x * fDeltaTime;
}

void CMoveObj::MoveY(float y)
{
	m_tPos.y += y;
}

void CMoveObj::MoveY(float y, float fDeltaTime)
{
	m_tPos.y += y * fDeltaTime;
}

void CMoveObj::MoveAngle(float fSpeed)
{
	m_tPos.x += cosf(m_fAngle) * fSpeed;
	m_tPos.y += sinf(m_fAngle) * fSpeed;
}

void CMoveObj::MoveAngle(float fSpeed, float fTime)
{
	m_tPos.x += cosf(m_fAngle) * fSpeed * fTime;
	m_tPos.y += sinf(m_fAngle) * fSpeed * fTime;
}

void CMoveObj::Input(float fDeltaTime)
{
}

int CMoveObj::Update(float fDeltaTime)
{
	return 0;
}

int CMoveObj::LateUpdate(float fDeletaTime)
{
	return 0;
}

void CMoveObj::Collision(float fDeltaTime)
{
}

void CMoveObj::Render(HDC hDC, float fDeltaTime)
{
}

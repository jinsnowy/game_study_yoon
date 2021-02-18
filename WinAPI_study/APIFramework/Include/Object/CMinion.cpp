#include "CMinion.h"
#include "../Core/CCore.h"
CMinion::CMinion()
{
	m_eDir = MD_FRONT;
}

CMinion::CMinion(const CMinion& obj)
	: CMoveObj(obj), m_eDir(obj.m_eDir)
{
}

CMinion::~CMinion()
{
}

bool CMinion::Init()
{
	SetPos(800.0f, 100.0f);
	SetSize(100.0f, 100.0f);
	SetSpeed(300.0f);

	return true;
}

void CMinion::Input(float fDeltaTime)
{
	CMoveObj::Input(fDeltaTime);

	MoveYFromSpeed(fDeltaTime, m_eDir);

	if (m_tPos.y + m_tSize.y >= GETRESOLUTION.iH)
	{
		m_tPos.y = GETRESOLUTION.iH - m_tSize.y;
		m_eDir = MD_BACK;
	}

	else if (m_tPos.y <= 0.f)
	{
		m_tPos.y = 0.f;
		m_eDir = MD_FRONT;
	}

}

int CMinion::Update(float fDeltaTime)
{
	CMoveObj::Update(fDeltaTime);
	return 0;
}

int CMinion::LateUpdate(float fDeltaTime)
{
	CMoveObj::LateUpdate(fDeltaTime);
	return 0;
}

void CMinion::Collision(float fDeltaTime)
{
	CMoveObj::Collision(fDeltaTime);
}

void CMinion::Render(HDC hDC, float fDeltaTime)
{
	CMoveObj::Render(hDC, fDeltaTime);
	Rectangle(hDC, m_tPos.x, m_tPos.y, m_tPos.x + m_tSize.x, m_tPos.y + m_tSize.y);
}

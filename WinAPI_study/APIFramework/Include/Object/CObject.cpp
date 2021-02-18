#include "CObject.h"

CObject::CObject()
{
}

CObject::CObject(const CObject& obj)
{
	*this = obj;
}

CObject::~CObject()
{
}

bool CObject::Init()
{
	return false;
}

void CObject::Input(float fDeltaTime)
{
}

int CObject::Update(float fDeltaTime)
{
	return 0;
}

int CObject::LateUpdate(float fDeletaTime)
{
	return 0;
}

void CObject::Collision(float fDeltaTime)
{
}

void CObject::Render(HDC hDC, float fDeltaTime)
{
}

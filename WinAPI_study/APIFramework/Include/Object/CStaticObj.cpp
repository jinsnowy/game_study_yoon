#include "CStaticObj.h"

CStaticObj::CStaticObj()
{
}

CStaticObj::CStaticObj(const CStaticObj& obj)
	: CObject(obj)
{
}

CStaticObj::~CStaticObj()
{
}

void CStaticObj::Input(float fDeltaTime)
{

}

int CStaticObj::Update(float fDeltaTime)
{
	return 0;
}

int CStaticObj::LateUpdate(float fDeletaTime)
{
	return 0;
}

void CStaticObj::Collision(float fDeltaTime)
{
}

void CStaticObj::Render(HDC hDC, float fDeltaTime)
{
}

#include "CLayer.h"
#include "../Object/CObject.h"

CLayer::CLayer() :
	m_iZOrder(0),
	m_strTag(""),
	m_pScene(nullptr)
{

}

CLayer::~CLayer()
{
	Safe_Release_VecList(m_ObjList);
}

void CLayer::AddObject(CObject* pObj)
{
	pObj->SetScene(m_pScene);
	pObj->SetLayer(this);
	pObj->AddRef();

	m_ObjList.push_back(pObj);
}


void CLayer::Input(float fDeltaTime)
{
	list<CObject*>::iterator iter;
	list<CObject*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		(*iter)->Input(fDeltaTime);
	}
}

int CLayer::Update(float fDeltaTime)
{
	list<CObject*>::iterator iter;
	list<CObject*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		(*iter)->Update(fDeltaTime);
	}
	return 0;
}

int CLayer::LateUpdate(float fDeltaTime)
{
	list<CObject*>::iterator iter;
	list<CObject*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		(*iter)->LateUpdate(fDeltaTime);
	}
	return 0;
}

void CLayer::Collision(float fDeltaTime)
{
	list<CObject*>::iterator iter;
	list<CObject*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		(*iter)->Collision(fDeltaTime);
	}
}


void CLayer::Render(HDC hDC, float fDeltaTime)
{
	list<CObject*>::iterator iter;
	list<CObject*>::iterator iterEnd = m_ObjList.end();

	for (iter = m_ObjList.begin(); iter != iterEnd; iter++)
	{
		(*iter)->Render(hDC, fDeltaTime);
	}
}

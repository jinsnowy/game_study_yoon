#pragma once
#include "../Game.h"
class CScene
{
protected:
	friend class CSceneManager;

protected:
	CScene();
	virtual ~CScene() = 0;
	list<class CLayer*> m_LayerList;

public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

	class CLayer* FindLayer(const string& strTag);
	CLayer* CreateLayer(const string& strTag, int iZOrder = 0);
	static bool LayerSort(class CLayer* pL1, class CLayer* pL2);
};
#pragma once
#include "CMoveObj.h"
class CPlayer : public CMoveObj
{
	friend class CObject;
private:
	CPlayer();
	CPlayer(const CPlayer& obj);
	~CPlayer();
public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
};


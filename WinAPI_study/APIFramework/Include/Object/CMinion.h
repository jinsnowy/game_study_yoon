#pragma once
#include "CMoveObj.h"
class CMinion : public CMoveObj
{
	friend class CObject;
private:
	MOVE_DIR m_eDir;
private:
	CMinion();
	CMinion(const CMinion& obj);
	~CMinion();
public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
};


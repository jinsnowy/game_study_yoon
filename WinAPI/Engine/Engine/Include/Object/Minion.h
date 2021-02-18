#pragma once
#include "MovableObject.h"
class Minion : public MovableObject
{
	friend class CObject;
private:
	MOVE_DIR m_Dir;
private:
	Minion();
	Minion(const Minion& obj);
	~Minion();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Render(HDC hDC, float dt);
};


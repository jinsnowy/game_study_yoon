#pragma once
#include "../framework.h"
#include "MovableObject.h"

class Player : public MovableObject
{
	friend class CObject;
private:
	Player();
	Player(const Player& obj);
	~Player();
public:
	virtual bool Init();
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeltaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);
};


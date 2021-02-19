#pragma once
#include "../framework.h"
#include "MovableObject.h"

class Player : public MovableObject
{
	friend class Object;
private:
	Player();
	Player(const Player& obj);
	~Player();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Player* Clone();

private:
	void Fire();

};


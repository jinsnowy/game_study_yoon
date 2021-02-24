#pragma once
#include "../framework.h"
#include "MovableObject.h"

class Player : public MovableObject
{
	friend class Object;
	friend class Scene;
private:
	Player();
	Player(const Player& obj);
	~Player();
private:
	int m_iHP = 0;
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Player* Clone();
public:
	void Hit(class Collider* pSrc, class Collider* pDst, float dt);
	void HitPixel(class Collider* pSrc, class Collider* pDst, float dt);
private:
	void Fire();
};


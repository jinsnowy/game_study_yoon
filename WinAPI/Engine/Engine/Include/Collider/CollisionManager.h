#pragma once
#include "../framework.h"

class CollisionManager
{
DECLARE_SINGLE(CollisionManager)
private:
	list <class Object*> m_CollisionList;
public:
	bool Init();
	void AddObject(class Object* pObj);
	void Collision(float dt);
};
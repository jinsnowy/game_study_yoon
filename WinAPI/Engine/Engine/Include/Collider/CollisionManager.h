#pragma once
#include "../framework.h"

class CollisionManager
{
DECLARE_SINGLE(CollisionManager)
private:
	list <class Object*> m_CollisionObjList;
public:
	bool Init();
	void AddObject(class Object* pObj);
	void Collision(float dt);
	bool CheckCollision(class Object* pSrc, class Object* pDst, float dt);
	void Clear();
};
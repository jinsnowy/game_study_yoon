#pragma once
#include "Collider.h"
class ColliderRect :
    public Collider
{
protected:
	ColliderRect();
	ColliderRect(const ColliderRect& coll);
	virtual ~ColliderRect();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual ColliderRect* Clone();
};


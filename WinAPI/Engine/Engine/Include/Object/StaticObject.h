#pragma once
#include "Object.h"

class StaticObject : public Object
{
protected:
	StaticObject();
	StaticObject(const StaticObject& obj);
	virtual ~StaticObject();
public:
	virtual bool Init() = 0;
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Render(HDC hDC, float dt);
};
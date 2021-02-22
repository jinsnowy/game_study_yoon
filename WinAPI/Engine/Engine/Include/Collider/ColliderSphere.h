#pragma once
#include "Collider.h"
#include "../framework.h"
class ColliderSphere : public Collider
{
	friend class Object;
protected:
	ColliderSphere();
	ColliderSphere(const ColliderSphere& coll);
	virtual ~ColliderSphere();
private:
	Sphere m_tInfo;
	Sphere m_tWorldInfo;
public:
	void SetSphere(const Pos& tCenter, float fRadius)
	{
		m_tInfo.center = tCenter;
		m_tInfo.radius = fRadius;
	}
	Sphere GetInfo() const
	{
		return m_tInfo;
	}
	Sphere GetWorldInfo() const
	{
		return m_tWorldInfo;
	}
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual bool CheckCollision(Collider* pDst);
	virtual void Draw(HDC hdc, float dt);
	virtual ColliderSphere* Clone();
};



#pragma once
#include "../Object/Ref.h"
#include "../framework.h"

class Collider : public Ref
{
	friend class Object;
protected:
	Collider();
	Collider(const Collider& coll);
	virtual ~Collider() = 0;
protected:
	COLLIDER_TYPE m_eCollType;
public:
	COLLIDER_TYPE GetColliderType() const
	{
		return m_eCollType;
	}
public:
	virtual bool Init() = 0;
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual Collider* Clone() = 0;
};


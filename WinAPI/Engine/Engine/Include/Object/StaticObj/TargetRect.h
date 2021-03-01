#pragma once
#include "StaticObject.h"
class TargetRect : public StaticObject
{
protected:
	TargetRect();
	TargetRect(const TargetRect& obj);
	virtual ~TargetRect();
	
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual TargetRect* Clone();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
private:
	class Object* m_pTarget;
public:
	void SetTarget(Object* obj);
};

#pragma once
#include "MovableObject.h"
class Bullet :
    public MovableObject
{
private:
    friend class Object;
private:
    Bullet();
    Bullet(const Bullet& bullet);
    ~Bullet();
private:
    float   m_Dist;
    float   m_LimitDist;
public:
    virtual bool Init();
    virtual void Input(float dt);
    virtual int Update(float dt);
    virtual int LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hDC, float dt);
    Bullet* Clone();
};


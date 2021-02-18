#pragma once
#include "Object.h"
class MovableObject :
    public Object
{
protected:
    MovableObject();
    MovableObject(const MovableObject& obj);
    virtual ~MovableObject();

private:
    float m_Angle;
    float m_Speed;

public:
    void SetAngle(float angle) { m_Angle = angle; }
    void SetSpeed(float speed) { m_Speed = speed; }
    float GetAngle() const { return m_Angle; }
    float GetSpeed() const { return m_Speed; }

    void Move(float x, float y);
    void Move(float x, float y, float dt);
    void Move(const Pos& tMove, float dt);

    void MoveXFromSpeed(float dt, MOVE_DIR dir);
    void MoveYFromSpeed(float dt, MOVE_DIR dir);

    void MoveX(float x);
    void MoveX(float x, float dt);
    void MoveY(float y);
    void MoveY(float y, float dt);

    void MoveAngle(float fSpeed);
    void MoveAngle(float fSpeed, float dt);
public:
    virtual bool Init() = 0;
    virtual void Input(float dt);
    virtual int Update(float dt);
    virtual int LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hDC, float dt);
    virtual MovableObject* Clone() = 0;
};


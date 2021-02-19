#include "Bullet.h"

Bullet::Bullet()
    : m_Dist(0.f),
      m_LimitDist(500.f)

{
}

Bullet::Bullet(const Bullet& bullet)
    : MovableObject(bullet),
    m_LimitDist(bullet.m_LimitDist),
    m_Dist(bullet.m_Dist)
{

}

Bullet::~Bullet()
{
}

bool Bullet::Init()
{
    return true;
}

void Bullet::Input(float dt)
{
    MovableObject::Input(dt);
}

int Bullet::Update(float dt)
{
    MovableObject::Update(dt);

    MoveAngle();

    m_Dist += GetSpeed() * dt;

    if (m_Dist >= m_LimitDist)
        Die();

    return 0;
}

int Bullet::LateUpdate(float dt)
{
    MovableObject::LateUpdate(dt);
    return 0;
}

void Bullet::Collision(float dt)
{
    MovableObject::Update(dt);
}

void Bullet::Draw(HDC hDC, float dt)
{
    MovableObject::Draw(hDC, dt);
    Ellipse(hDC, (int)m_Pos.x, (int)m_Pos.y, int(m_Pos.x + m_Size.x), int(m_Pos.y + m_Size.y));
}

Bullet* Bullet::Clone()
{
    return new Bullet(*this);
}

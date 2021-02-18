#include "Object.h"

Object::Object():
    m_Pos(0,0),
    m_Size(0,0)
{
}

Object::~Object()
{
}

bool Object::Init()
{
    return false;
}

void Object::Input(float dt)
{
}

void Object::Update(float dt)
{
}

void Object::LateUpdate(float dt)
{
}

void Object::Collision(float dt)
{
}

void Object::Draw(HDC hdc, float dt)
{
}

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

int Object::Update(float dt)
{
    return 0;
}

int Object::LateUpdate(float dt)
{
    return 0;
}

void Object::Collision(float dt)
{
}

void Object::Draw(HDC hdc, float dt)
{
}

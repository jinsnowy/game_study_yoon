#include "StaticObject.h"

StaticObject::StaticObject()
{
}

StaticObject::StaticObject(const StaticObject& obj)
    : Object(obj)
{
}

StaticObject::~StaticObject()
{
}

bool StaticObject::Init()
{
    return false;
}

void StaticObject::Input(float dt)
{
}

int StaticObject::Update(float dt)
{
    return 0;
}

int StaticObject::LateUpdate(float dt)
{
    return 0;
}

void StaticObject::Collision(float dt)
{
}

void StaticObject::Draw(HDC hDC, float dt)
{
}

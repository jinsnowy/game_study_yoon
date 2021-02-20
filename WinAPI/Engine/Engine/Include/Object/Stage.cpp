#include "Stage.h"

Stage::Stage()
{
}

Stage::Stage(const Stage& obj)
    :
    StaticObject(obj)
{
}

Stage::~Stage()
{
}

bool Stage::Init()
{
    StaticObject::Init();
    return true;
}

void Stage::Input(float dt)
{
    StaticObject::Input(dt);
}

int Stage::Update(float dt)
{
    StaticObject::Update(dt);
    return 0;
}

int Stage::LateUpdate(float dt)
{
    StaticObject::LateUpdate(dt);
    return 0;
}

void Stage::Collision(float dt)
{
    StaticObject::Collision(dt);
}

void Stage::Draw(HDC hDC, float dt)
{
  
    StaticObject::Draw(hDC, dt);
}

Stage* Stage::Clone()
{
    return new Stage(*this);
}

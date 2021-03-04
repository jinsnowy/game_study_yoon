#include "TileObject.h"

TileObject::TileObject()
{
}

TileObject::TileObject(const TileObject& obj)
    :StaticObject(obj)
{
    m_iTileIndex = obj.m_iTileIndex;
}

TileObject::~TileObject()
{
}

bool TileObject::Init()
{
    return false;
}

void TileObject::Input(float dt)
{
    StaticObject::Input(dt);
}

int TileObject::Update(float dt)
{
    StaticObject::Update(dt);
    return 0;
}

int TileObject::LateUpdate(float dt)
{
    StaticObject::LateUpdate(dt);
    return 0;
}

void TileObject::Collision(float dt)
{
    StaticObject::Collision(dt);
}

void TileObject::Draw(HDC hDC, float dt)
{
    StaticObject::Draw(hDC, dt);
}

void TileObject::Save(FILE* pFile)
{
    StaticObject::Save(pFile);
}

void TileObject::Load(FILE* pFile)
{
    StaticObject::Load(pFile);
}

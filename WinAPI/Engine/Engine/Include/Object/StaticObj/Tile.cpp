#include "Tile.h"

Tile::Tile():
	m_eOption(TO_NONE)
{
}

Tile::Tile(const Tile& tile)
{
}


Tile::~Tile()
{
}

bool Tile::Init()
{
	return true;
}

void Tile::Input(float dt)
{
	StaticObject::Input(dt);
}

int Tile::Update(float dt)
{
	StaticObject::Update(dt);
	return 0;
}

int Tile::LateUpdate(float dt)
{
	StaticObject::Update(dt);
	return 0;
}

void Tile::Collision(float dt)
{
	StaticObject::Collision(dt);
}

void Tile::Draw(HDC hDC, float dt)
{
	StaticObject::Draw(hDC, dt);
}

Tile* Tile::Clone()
{
	return new Tile(*this);
}

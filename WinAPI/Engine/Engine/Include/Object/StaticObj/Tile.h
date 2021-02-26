#pragma once
#include "StaticObject.h"
class Tile :
    public StaticObject
{
private:
    friend class Object;
    friend class Stage;
private:
	TILE_OPTION m_eOption;
public:
	void SetTileOption(TILE_OPTION eOption)
	{
		m_eOption = eOption;
	}
	TILE_OPTION GetTileOption() const
	{
		return m_eOption;
	}
private:
    Tile();
    Tile(const Tile& tile);
    ~Tile();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Tile* Clone();
};


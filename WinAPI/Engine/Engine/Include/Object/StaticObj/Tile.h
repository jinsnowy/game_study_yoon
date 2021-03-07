#pragma once
#include "StaticObject.h"
class Tile :
    public StaticObject
{
private:
    friend class Object;
    friend class Stage;
private:
	TILE_OPTION		m_eOption;
	class Texture*	m_pOptionTex = nullptr;
public:
	void SetTileOption(TILE_OPTION eOption);
	TILE_OPTION GetTileOption() const
	{
		return m_eOption;
	}
public:
    Tile();
    Tile(const Tile& tile);
    virtual ~Tile();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Tile* Clone();
private:
	void DrawRemains(HDC hDC, float dt);
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void LateInit();
};


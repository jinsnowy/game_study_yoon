#pragma once
#include "StaticObject.h"
class Tile :
    public StaticObject
{
private:
    friend class Object;
	friend class Layer;
    friend class Stage;
protected:
	TILE_TYPE		m_eType = TL_NONE;
	TILE_OPTION		m_eOption = TO_NONE;
	class Texture*	m_pOptionTex = nullptr;
public:
	TILE_TYPE GetTileType() const { return m_eType; }
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


#pragma once
#include "StaticObject.h"

class Stage : public StaticObject
{
	friend class MapEditScene;
	friend class Object;
	friend class Scene;
private:
	Stage();
	Stage(const Stage& obj);
	~Stage();
	void DrawBackGround(HDC hdc, COLORREF color);
private:
	int m_iTileNumX;
	int m_iTileNumY;
	int m_iTileSizeX;
	int m_iTileSizeY;
	vector<class Tile*> m_baseTile;
public:
	void CreateTile(int iNumX, int iNumY, int iSizeX, int iSizeY, Pos tPivot = Pos(0.f, 1.0f));
public:

	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Stage* Clone();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
public:
	TILE_OPTION GetTileOption(const Pos& pos) const;
	string GetTileName(const Pos& pos) const;
	void SetTileNone(const Pos& tPos);
	void ChangeTileTexture(const Pos& tPos, class Texture* pTexture);
	void ChangeTileOption(const Pos& tPos, TILE_OPTION eOption);
	INDEX GetTileRowColIndex(const Pos& tPos) const;
	INDEX GetTileRowColIndex(float x, float y) const;
	int GetTileIndex(const Pos& tPos) const;
	int GetTileIndex(float x, float y) const;
	RESOLUTION GetTileSize() const { return { m_iTileSizeX, m_iTileSizeY }; }
	int GetStageWidth() const { return m_iTileNumX * m_iTileSizeX; }
	int GetStageHeight() const { return m_iTileNumY * m_iTileSizeY; }
private:
	void ClearTile();
};


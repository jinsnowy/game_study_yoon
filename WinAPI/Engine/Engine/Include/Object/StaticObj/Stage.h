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
private:
	int m_iTileNumX;
	int m_iTileNumY;
	vector<class Tile*> m_baseTile;
public:
	void CreateTile(int iNumX, int iNumY);
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
	void ChangeTileByCloneTile(const Pos& tPos, Tile* pClone);
	void ChangeTileOption(const Pos& tPos, TILE_OPTION eOption);

	INDEX GetTileRowColIndex(const Pos& tPos) const;
	INDEX GetTileRowColIndex(float x, float y) const;
	int GetTileIndex(const Pos& tPos) const;
	int GetTileIndex(float x, float y) const;
	RESOLUTION GetTileSize() const { return { TILESIZE, TILESIZE }; }
	int GetStageWidth() const { return m_iTileNumX * TILESIZE; }
	int GetStageHeight() const { return m_iTileNumY * TILESIZE; }
	int GetStageTileNumX() const { return m_iTileNumX; }
	int GetStageTileNumY() const { return m_iTileNumY; }
private:
	void ClearTile();
};


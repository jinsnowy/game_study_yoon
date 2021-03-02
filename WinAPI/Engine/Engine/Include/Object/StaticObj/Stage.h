#pragma once
#include "StaticObject.h"

class Stage : public StaticObject
{
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
	Texture* m_baseTexture = nullptr;
	vector<class Tile*> m_baseTile;
public:
	void CreateTile(int iNumX, int iNumY, int iSizeX, int iSizeY,
		const string& strKey = "", const wchar_t* pFileName = nullptr, const string& strPathKey = TEXTURE_PATH);
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Stage* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
public:
	Texture* GetBaseTexture()const { return m_baseTexture; }
	void SetTileNone(const Pos& tPos);
	void ChangeTileTexture(const Pos& tPos, class Texture* pTexture);
	void ChangeTileOption(const Pos& tPos, TILE_OPTION eOption);
	int GetTileIndex(const Pos& tPos);
	int GetTileIndex(float x, float y);
	RESOLUTION GetTileSize() const { return { m_iTileSizeX, m_iTileSizeY }; }
private:
	void ClearTile();
};


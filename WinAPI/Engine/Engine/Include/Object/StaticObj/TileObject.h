#pragma once
#include "StaticObject.h"
class TileObject : public StaticObject
{
protected:
	TileObject();
	TileObject(const TileObject& obj);
	virtual ~TileObject() = 0;
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual TileObject* Clone() = 0;
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
private:
	int m_iTileIndex;
public:
	int GetTileIndex() const { return m_iTileIndex; }
};
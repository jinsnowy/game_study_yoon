#pragma once
#include "Tile.h"

class Tree : public Tile
{
	friend class PrototypeManager;
private:
	static constexpr int m_iTypeNum = 3;
	static constexpr wchar_t m_strFolderPath[] = L"SV/Object/Plant/Tree/";
	static const wchar_t* const m_strBaseName[];
protected:
	Tree();
	Tree(const Tree& obj);
	virtual ~Tree();
private:
	void ChangeTreeTexture(int id);
	void ShadeIn(Collider* pSrc, Collider* pDst, float dt);
	void ShadeOut(Collider* pSrc, Collider* pDst, float dt);
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Tree* Clone();
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void LateInit();
};


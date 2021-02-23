#pragma once
#include "Collider.h"

typedef struct _tagPixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
}Pixel;

class ColliderPixel :
    public Collider
{
	friend class Object;
protected:
	ColliderPixel();
	ColliderPixel(const ColliderPixel& coll);
	virtual ~ColliderPixel();
private:
	int m_iWidth;
	int m_iHeight;
	vector<Pixel> m_vecPixel;
public:
	bool SetPixelInfo(char* pFileName, const string& strPathKey = TEXTURE_PATH);
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual bool CheckCollision(Collider* pDst);
	virtual void Draw(HDC hdc, float dt);
	virtual ColliderPixel* Clone();
};



#include "Tree.h"
#include "Tree.h"
#include "../../Resources/ResourceManager.h"
const wchar_t* const Tree::m_strBaseName[] = { L"Tree1.bmp", L"Tree2.bmp", L"Tree3.bmp" };

Tree::Tree()
{
}

Tree::Tree(const Tree& obj)
	: StaticObject(obj)
{
}

Tree::~Tree()
{
}

bool Tree::Init()
{
	wchar_t path[MAX_PATH] = { 0 };
	wcscat(path, m_strFolderPath);
	wcscat(path, m_strBaseName[0]);


	SetTexture("TreeTexture1", path);
	SetColorKey(255, 255, 255);
	SetAsTextureSize();

	Size imgSize = GetImageSize();
	SetPos(0.f, 0.f);
	SetPivot(0.3333f, 1.0f);

	return true;
}

void Tree::ChangeTreeTexture(int id)
{
	assert(id < m_iTypeNum);
	string strTexKey = "TreeTexture" + to_string(id + 1);
	wchar_t path[MAX_PATH] = { 0 };
	wcscat(path, m_strFolderPath);
	wcscat(path, m_strBaseName[id]);
	SetTexture(strTexKey, path);
	SetColorKey(255, 255, 255);
}

void Tree::Input(float dt)
{
	StaticObject::Input(dt);
}

int Tree::Update(float dt)
{
	StaticObject::Update(dt);
	return 0;
}

int Tree::LateUpdate(float dt)
{
	StaticObject::LateUpdate(dt);
	return 0;
}

void Tree::Collision(float dt)
{
	StaticObject::Collision(dt);
}

void Tree::Draw(HDC hDC, float dt)
{
	StaticObject::Draw(hDC, dt);
}

Tree* Tree::Clone()
{
	return new Tree(*this);
}

void Tree::Save(FILE* pFile)
{
	StaticObject::Save(pFile);
}

void Tree::Load(FILE* pFile)
{
	StaticObject::Load(pFile);
}

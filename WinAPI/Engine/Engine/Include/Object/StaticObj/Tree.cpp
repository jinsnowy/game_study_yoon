#include "Tree.h"
#include "Tree.h"
#include "../../Resources/ResourceManager.h"
#include "../../Collider/ColliderRect.h"
#include "../../Resources/Texture.h"
const wchar_t* const Tree::m_strBaseName[] = { L"Tree1.bmp", L"Tree2.bmp", L"Tree3.bmp" };

Tree::Tree()
{
}

Tree::Tree(const Tree& obj)
	: Tile(obj)
{
}

Tree::~Tree()
{
}

bool Tree::Init()
{
	wchar_t path[MAX_PATH] = { 0 };
	wcscat_s(path, m_strFolderPath);
	wcscat_s(path, m_strBaseName[0]);

	SetTexture("TreeTexture1", path);
	SetColorKey(255, 255, 255);
	SetAsTextureSize();

	Size imgSize = GetImageSize();
	SetPos(0.f, 0.f);
	SetPivot(0.3333f, 1.0f);

	ColliderRect* pRC = AddCollider<ColliderRect>("TreeBody");
	pRC->SetRect(-imgSize.x / 3 + 2, -imgSize.y + TILESIZE + 2, 2 * imgSize.x / 3 - 2, TILESIZE - 2);
	pRC->AddCollisionFunction(CS_ENTER, this, &Tree::ShadeIn);
	pRC->AddCollisionFunction(CS_STAY, this, &Tree::ShadeIn);
	SAFE_RELEASE(pRC);
	return true;
}

void Tree::LateInit()
{
	StaticObject::LateInit();
	ColliderRect* pRC = static_cast<ColliderRect*>(GetCollider("TreeBody"));
	pRC->AddCollisionFunction(CS_ENTER, this, &Tree::ShadeIn);
	pRC->AddCollisionFunction(CS_STAY, this, &Tree::ShadeIn);
	// pRC->AddCollisionFunction(CS_LEAVE, this, &Tree::CollideFunc);
	SAFE_RELEASE(pRC);
}

void Tree::ChangeTreeTexture(int id)
{
	assert(id < m_iTypeNum);
	string strTexKey = "TreeTexture" + to_string(id + 1);
	wchar_t path[MAX_PATH] = { 0 };
	wcscat_s(path, m_strFolderPath);
	wcscat_s(path, m_strBaseName[id]);
	SetTexture(strTexKey, path);
	SetColorKey(255, 255, 255);
}

void Tree::ShadeIn(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "PlayerBody")
	{
		ColliderRect* pRC_tree = static_cast<ColliderRect*>(pSrc);
		ColliderRect* pRC_player = static_cast<ColliderRect*>(pDst);
		if (pRC_tree->GetWorldInfo().bottom > pRC_player->GetWorldInfo().bottom)
		{
			m_pTexture->EnableTransparentEffect();
		}
	}
}
void Tree::ShadeOut(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "PlayerBody")
	{
		m_pTexture->DisableTransparentEffect();
	}
}
void Tree::Input(float dt)
{

	Tile::Input(dt);
}

int Tree::Update(float dt)
{
	Tile::Update(dt);
	return 0;
}

int Tree::LateUpdate(float dt)
{
	Tile::LateUpdate(dt);
	return 0;
}

void Tree::Collision(float dt)
{
	Tile::Collision(dt);
}

void Tree::Draw(HDC hDC, float dt)
{
	Tile::Draw(hDC, dt);
}

Tree* Tree::Clone()
{
	return new Tree(*this);
}

void Tree::Save(FILE* pFile)
{
	Tile::Save(pFile);
}

void Tree::Load(FILE* pFile)
{
	Tile::Load(pFile);
}

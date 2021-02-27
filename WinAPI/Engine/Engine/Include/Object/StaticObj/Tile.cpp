#include "Tile.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/Texture.h"
#include "../../Core/Camera.h"

Tile::Tile():
	m_eOption(TO_NONE)
{
	m_pOptionTex = RESOURCE_MANAGER->FindTexture("TileNone");
}

Tile::Tile(const Tile& tile)
	:
	StaticObject(tile)
{
	m_eOption = tile.m_eOption;
	m_pOptionTex = tile.m_pOptionTex;

	if(m_pOptionTex)
		m_pOptionTex->AddRef();
}

Tile::~Tile()
{
	SAFE_RELEASE(m_pOptionTex);
}

void Tile::SetTileOption(TILE_OPTION eOption)
{
	m_eOption = eOption;
	SAFE_RELEASE(m_pOptionTex);
	switch (eOption)
	{
	case TO_NONE:
		m_pOptionTex = RESOURCE_MANAGER->FindTexture("TileNone");
		break;
	case TO_NOMOVE:
		m_pOptionTex = RESOURCE_MANAGER->FindTexture("TileNoMove");
		break;
	}
}
bool Tile::Init()
{
	return true;
}

void Tile::Input(float dt)
{
	StaticObject::Input(dt);
}

int Tile::Update(float dt)
{
	StaticObject::Update(dt);
	return 0;
}

int Tile::LateUpdate(float dt)
{
	StaticObject::Update(dt);
	return 0;
}

void Tile::Collision(float dt)
{
	StaticObject::Collision(dt);
}

void Tile::Draw(HDC hDC, float dt)
{
	StaticObject::Draw(hDC, dt);

    Pos tPos = m_tPos - m_tSize * m_tPivot;
    tPos -= CAMERA->GetTopLeft();
    // 카메라 컬링
    RESOLUTION tClientRS = CAMERA->GetClientRS();
    if (tPos.x + m_tSize.x < 0 || tPos.x > tClientRS.x || tPos.y + m_tSize.y < 0 || tPos.y > tClientRS.y)
        return;

    if (m_pOptionTex)
    {
        Pos tImagePos = m_tImageOffset;

        if (m_pOptionTex->GetColorKeyEnable())
        {
            TransparentBlt(hDC, int(tPos.x), int(tPos.y), int(m_tSize.x), int(m_tSize.y),
				m_pOptionTex->GetDC(), int(tImagePos.x), int(tImagePos.y),
                int(m_tSize.x), int(m_tSize.y),
				m_pOptionTex->GetColorKey());
        }
        else
        {
            BitBlt(hDC, int(tPos.x), int(tPos.y), int(m_tSize.x), int(m_tSize.y),
				m_pOptionTex->GetDC(), int(tImagePos.x), int(tImagePos.y), SRCCOPY);
        }
    }
}

Tile* Tile::Clone()
{
	return new Tile(*this);
}

void Tile::Save(FILE* pFile)
{
	StaticObject::Save(pFile);

	fwrite(&m_eOption, 4, 1, pFile);

}

void Tile::Load(FILE* pFile)
{
	StaticObject::Load(pFile);

	fread(&m_eOption, 4, 1, pFile);

	SetTileOption(m_eOption);
}

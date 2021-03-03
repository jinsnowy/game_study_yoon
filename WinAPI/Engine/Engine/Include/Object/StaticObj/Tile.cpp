#include "Tile.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/Texture.h"
#include "../../Core/Camera.h"
#include "../../Animation/Animation.h"

Tile::Tile():
	m_eOption(TO_NONE)
{
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

void Tile::ReleaseTexture()
{
	SAFE_RELEASE(m_pTexture);
    SAFE_RELEASE(m_pOptionTex);
    m_eOption = TO_NONE;
}

void Tile::SetTileOption(TILE_OPTION eOption)
{
	m_eOption = eOption;
	SAFE_RELEASE(m_pOptionTex);
	switch (eOption)
	{
	//case TO_NONE:
	//	m_pOptionTex = RESOURCE_MANAGER->FindTexture("1.NoOption");
	//	break;
	case TO_NOMOVE:
		m_pOptionTex = RESOURCE_MANAGER->FindTexture("2.NoMove");
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
    Pos tPos = m_tPos - m_tSize * m_tPivot;
    tPos -= CAMERA->GetTopLeft();
    // ī�޶� �ø�
    RESOLUTION tClientRS = CAMERA->GetClientRS();
    if (tPos.x + m_tSize.x < 0 || tPos.x > tClientRS.x || tPos.y + m_tSize.y < 0 || tPos.y > tClientRS.y)
        return;

    if (m_pTexture)
    {

        Pos tImagePos = m_tImageOffset;
        if (m_pAnimation && m_bEnableAnimation)
        {
            AnimationClip* pClip = m_pAnimation->GetCurrentClip();

            switch (pClip->eType)
            {
            case AT_FRAME:
                SetTexture(pClip->vecTexture[pClip->iFrameX]);
                break;
            case AT_ATLAS:
                tImagePos.x = pClip->iFrameX * m_tSize.x + m_tImageOffset.x;
                tImagePos.y = pClip->iFrameY * m_tSize.y + m_tImageOffset.y;
                break;
            }
        }

        Size tSize = GetImageSize();
        if (m_pTexture->GetColorKeyEnable())
        {
            TransparentBlt(hDC, int(tPos.x), int(tPos.y), int(tSize.x), int(tSize.y),
                m_pTexture->GetDC(), int(tImagePos.x), int(tImagePos.y),
                int(tSize.x), int(tSize.y),
                m_pTexture->GetColorKey());
        }
        else
        {
            BitBlt(hDC, int(tPos.x), int(tPos.y), int(tSize.x), int(tSize.y),
                m_pTexture->GetDC(), int(tImagePos.x), int(tImagePos.y), SRCCOPY);
        }
    }

    list<Collider*>::iterator iter;
    list<Collider*>::iterator iterEnd = m_ColliderList.end();

    for (iter = m_ColliderList.begin(); iter != iterEnd;)
    {
        if (!(*iter)->GetEnable())
        {
            ++iter;
            continue;
        }

        (*iter)->Draw(hDC, dt);

        if (!(*iter)->GetLife())
        {
            SAFE_RELEASE((*iter));
            iter = m_ColliderList.erase(iter);
            iterEnd = m_ColliderList.end();
        }
        else ++iter;
    }

    if (m_pOptionTex)
    {
        Pos tImagePos = m_tImageOffset;

        if (m_pOptionTex->GetColorKeyEnable())
        {
            TransparentBlt(hDC, int(tPos.x), int(tPos.y), TILESIZE, TILESIZE,
				m_pOptionTex->GetDC(), int(tImagePos.x), int(tImagePos.y),
                TILESIZE, TILESIZE,
				m_pOptionTex->GetColorKey());
        }
        else
        {
            BitBlt(hDC, int(tPos.x), int(tPos.y), TILESIZE, TILESIZE,
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

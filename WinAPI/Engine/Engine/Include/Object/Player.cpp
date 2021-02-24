#include "Player.h"
#include "Bullet.h"
#include "../Math.h"
#include "../Core/Input.h"
#include "../Collider/ColliderRect.h"
#include "../Collider/ColliderPixel.h"
#include "../Core/Camera.h"

Player::Player()
{
}

Player::Player(const Player& obj) : MovableObject(obj)
{
}

Player::~Player()
{
}

bool Player::Init()
{
	SetPos(50.0f, 50.0f);
	SetSize(100.0f, 100.0f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(400.0f);

	SetTexture("Player", L"hos.bmp");

	m_iHP = 1000;

	// 중력을 적용한다.
	SetPhysics(true);

	// 점프할 힘을 설정한다.
	SetForce(600.f);

	ColliderRect* pRC = AddCollider<ColliderRect>("PlayerBody");
	pRC->SetRect(-50.f, -50.f, 50.f, 50.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &Player::Hit);
	
	SAFE_RELEASE(pRC);

	ColliderRect* pRC_bot = AddCollider<ColliderRect>("PlayerBottom");
	pRC_bot->SetRect(-5.f, 25.f, 5.f, 75.f);
	pRC_bot->AddCollisionFunction(CS_ENTER, this, &Player::HitPixel);
	pRC_bot->AddCollisionFunction(CS_STAY, this, &Player::HitPixel);

	SAFE_RELEASE(pRC_bot);

	return true;
}

void Player::Input(float dt)
{
	MovableObject::Input(dt);
	if (KEYDOWN("MoveBack"))
	{
		// MoveYFromSpeed(dt, MD_FRONT);
	}
	if (KEYDOWN("MoveFront"))
	{
		Jump();
		// MoveYFromSpeed(dt, MD_BACK);
	}
	if (KEYPRESS("MoveLeft"))
	{
		MoveXFromSpeed(dt, MD_BACK);
	}
	if (KEYPRESS("MoveRight"))
	{
		MoveXFromSpeed(dt, MD_FRONT);
	}
	if (KEYDOWN("Fire"))
	{
		Fire();
	}
	if (KEYDOWN("Skill1"))
	{
		MessageBox(NULL, L"Skill1", L"Skill1", MB_OK);
	}
}

int Player::Update(float dt)
{
	MovableObject::Update(dt);
	return 0;
}

int Player::LateUpdate(float dt)
{
	MovableObject::LateUpdate(dt);
	return 0;
}

void Player::Collision(float dt)
{
	MovableObject::Collision(dt);
}

void Player::Draw(HDC hDC, float dt)
{
  	MovableObject::Draw(hDC, dt);
	wchar_t strHP[32] = {};
	swprintf_s(strHP, L"HP: %d", m_iHP);
	Pos tPos = m_Pos - m_Size * m_Pivot;
	tPos -= CAMERA.GetTopLeft();
	TextOut(hDC, tPos.x, tPos.y, strHP, lstrlen(strHP));
}

Player* Player::Clone()
{
	return new Player(*this);
}

void Player::Hit(Collider* pSrc, Collider* pDst, float dt)
{
	if(pDst->GetObj()->GetTag() == "MinionBullet")
		m_iHP -= 5;
}


void Player::HitPixel(Collider* pSrc, Collider* pDst, float dt)
{
	if (pDst->GetTag() == "StageColl")
	{
		const vector<Pixel>& pixels = static_cast<ColliderPixel*>(pDst)->GetPixel();
		int iWidth = static_cast<ColliderPixel*>(pDst)->GetWidth();
		int iHeight = static_cast<ColliderPixel*>(pDst)->GetHeight();

		Rect src = static_cast<ColliderRect*>(pSrc)->GetWorldInfo();
		Pos tPos = static_cast<ColliderRect*>(pSrc)->GetObj()->GetPos();

		int iStartX, iEndX;
		int iStartY, iEndY;

		iStartX = src.left < 0 ? 0 : src.left;
		iEndX = src.right >= iWidth ? iWidth - 1 : src.right;

		iStartY = src.top < 0 ? 0 : src.top;
		iEndY = src.bottom >= iHeight ? iHeight - 1 : src.bottom;

		int outer_most_y = INT_MAX;
		for (int i = iStartY; i <= iEndY; ++i)
		{
			for (int j = iStartX; j <= iEndX; ++j)
			{
				int ind = i * iWidth + j;
				const Pixel& pixel = pixels[ind];
				if (pixel.r == 255 && pixel.g == 0 && pixel.b == 255)
				{
					if (i < outer_most_y)
					{
						outer_most_y = i;
					}
				}
			}
		}
		int bottom = (src.top + src.bottom) / 2;
		if (outer_most_y < bottom)
		{
			if (!IsMoveUp())
			{
				SetPos(GetPos().x, GetPos().y - (bottom - outer_most_y));
				JumpEnd();
			}
		}
		OnGround();
	}
}

void Player::Fire()
{
	Object* pBullet = Object::CreateCloneObject("Bullet", "PlayerBullet", m_pLayer);

	pBullet->AddCollisionFunction("BulletBody", CS_ENTER, (Bullet*) pBullet, &Bullet::Hit);

	Pos tPos;
	tPos.x = GetRight() + pBullet->GetPivot().x * pBullet->GetSize().x;
	tPos.y = GetCenter().y;

	pBullet->SetPos(tPos.x, tPos.y - pBullet->GetSize().y / 2.f);

	SAFE_RELEASE(pBullet);
}
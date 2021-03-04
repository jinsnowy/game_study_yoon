#include <cassert>
#include "Player.h"
#include "Bullet.h"
#include "../../Math.h"
#include "../../Scene/Scene.h"
#include "../../Core/Input.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/Texture.h"
#include "../../Collider/ColliderRect.h"
#include "../../Collider/ColliderPixel.h"
#include "../../Core/Camera.h"
#include "../../Animation/Animation.h"

Player::Player()
{
}

Player::Player(const Player& obj) : MovableObject(obj)
{
}

Player::~Player()
{
}

void Player::InitTexture()
{
	Texture* pTex;
	pTex = RESOURCE_MANAGER->LoadTexture("IdleRight", L"Player/SV/IdleRight.bmp");
	pTex->SetColorKey(255, 255, 255);
	SAFE_RELEASE(pTex);
	pTex = RESOURCE_MANAGER->LoadTexture("IdleLeft", L"Player/SV/IdleLeft.bmp");
	pTex->SetColorKey(255, 255, 255);
	SAFE_RELEASE(pTex);
	pTex = RESOURCE_MANAGER->LoadTexture("IdleDown", L"Player/SV/IdleDown.bmp");
	pTex->SetColorKey(255, 255, 255);
	SAFE_RELEASE(pTex);
	pTex = RESOURCE_MANAGER->LoadTexture("IdleUp", L"Player/SV/IdleUp.bmp");
	pTex->SetColorKey(255, 255, 255);
	SAFE_RELEASE(pTex);
}

void Player::InitAnimation()
{
	SetEnableAnimation(true);
	Animation* pAnim = CreateAnimation("PlayerAnimation");
	SAFE_RELEASE(pAnim);
	wstring basePath = L"Player/SV/WalkRight/";
	vector<wstring> vecFileName;
	wstringstream ss;

	vecFileName.clear();
	for (int i = 1; i <= 6; ++i)
	{
		ss << basePath << L"WalkRight" << i << ".bmp";
		vecFileName.push_back(ss.str());
		ss.clear();
		ss.str(L"");
	}
	AddAnimationClip("WalkRight",
		AT_FRAME, AO_LOOP,
		0.0f, 0.55f,
		6, 1,
		0, 0,
		6, 1,
		0.f, "WalkRight_Anim", vecFileName);
	SetClipColorKey("WalkRight", 255, 255, 255);

	basePath = L"Player/SV/WalkLeft/";
	vecFileName.clear();
	for (int i = 1; i <= 6; ++i)
	{
		ss << basePath << L"WalkLeft" << i << ".bmp";
		vecFileName.push_back(ss.str());
		ss.clear();
		ss.str(L"");
	}
	AddAnimationClip("WalkLeft",
		AT_FRAME, AO_LOOP,
		0.0f, 0.55f,
		6, 1,
		0, 0,
		6, 1,
		0.f, "WalkLeft_Anim", vecFileName);
	SetClipColorKey("WalkLeft", 255, 255, 255);

	basePath = L"Player/SV/WalkDown/";
	vecFileName.clear();
	for (int i = 1; i <= 9; ++i)
	{
		ss << basePath << L"WalkDown" << i << ".bmp";
		vecFileName.push_back(ss.str());
		ss.clear();
		ss.str(L"");
	}
	AddAnimationClip("WalkDown",
		AT_FRAME, AO_LOOP,
		0.0f, 0.55f,
		9, 1,
		0, 0,
		9, 1,
		0.f, "WalkDown_Anim", vecFileName);
	SetClipColorKey("WalkDown", 255, 255, 255);

	basePath = L"Player/SV/WalkUp/";
	vecFileName.clear();
	for (int i = 1; i <= 9; ++i)
	{
		ss << basePath << L"WalkUp" << i << ".bmp";
		vecFileName.push_back(ss.str());
		ss.clear();
		ss.str(L"");
	}
	AddAnimationClip("WalkUp",
		AT_FRAME, AO_LOOP,
		0.0f, 0.55f,
		9, 1,
		0, 0,
		9, 1,
		0.f, "WalkUp_Anim", vecFileName);
	SetClipColorKey("WalkUp", 255, 255, 255);
}

void Player::SetState(PlayerState state)
{
	m_eState = state;
	switch (m_eState)
	{
	case IDLE_LEFT:
	case IDLE_RIGHT:
	case IDLE_UP:
	case IDLE_DOWN:
		m_bEnableAnimation = false;
		break;
	case WALK_LEFT:
	case WALK_RIGHT:
	case WALK_DOWN:
	case WALK_UP:
		m_bEnableAnimation = true;
		break;
	}
	switch (m_eState)
	{
	case IDLE_LEFT:
		SetTexture("IdleLeft");
		break;
	case IDLE_RIGHT:
		SetTexture("IdleRight");
		break;
	case IDLE_UP:
		SetTexture("IdleUp");
		break;
	case IDLE_DOWN:
		SetTexture("IdleDown");
		break;
	case WALK_LEFT:
		m_pAnimation->ChangeClip("WalkLeft");
		break;
	case WALK_RIGHT:
		m_pAnimation->ChangeClip("WalkRight");
		break;
	case WALK_DOWN:
		m_pAnimation->ChangeClip("WalkDown");
		break;
	case WALK_UP:
		m_pAnimation->ChangeClip("WalkUp");
		break;
	}
}

bool Player::Init()
{
	SetPos(400.0f, 500.0f);
	SetSize(64.0f, 128.0f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(300.0f);

	InitTexture();
	InitAnimation();

	SetState(IDLE_RIGHT);
	m_iHP = 1000;

	// 중력을 적용한다.
	SetPhysics(false);

	// 점프할 힘을 설정한다.
	SetForce(500.f);

	ColliderRect* pRC = AddCollider<ColliderRect>("PlayerBody");
	pRC->SetRect(-40.f, -50.f, 40.f, 50.f);
	pRC->AddCollisionFunction(CS_ENTER, this, &Player::Hit);
	
	SAFE_RELEASE(pRC);

	ColliderRect* pRC_bot = AddCollider<ColliderRect>("PlayerBottom");
	pRC_bot->SetRect(-5.f, 49.f, 5.f, 51.f);
	pRC_bot->AddCollisionFunction(CS_ENTER, this, &Player::HitPixel);
	pRC_bot->AddCollisionFunction(CS_STAY, this, &Player::HitPixel);

	SAFE_RELEASE(pRC_bot);

	return true;
}

void Player::Input(float dt)
{
	MovableObject::Input(dt);
	if (KEYPRESS("MoveUp"))
	{
		MoveYFromSpeed(dt, MD_BACK);
		SetState(WALK_UP);
	}
	else if (KEYPRESS("MoveDown"))
	{
		MoveYFromSpeed(dt, MD_FRONT);
		SetState(WALK_DOWN);
	}
	else if (KEYPRESS("MoveLeft"))
	{
		MoveXFromSpeed(dt, MD_BACK);
		SetState(WALK_LEFT);
	}
	else if (KEYPRESS("MoveRight"))
	{
		MoveXFromSpeed(dt, MD_FRONT);
		SetState(WALK_RIGHT);
	}
	else
	{
		switch (m_eState)
		{
		case WALK_RIGHT:
			SetState(IDLE_RIGHT);
			break;
		case WALK_LEFT:
			SetState(IDLE_LEFT);
			break;
		case WALK_UP:
			SetState(IDLE_UP);
			break;
		case WALK_DOWN:
			SetState(IDLE_DOWN);
			break;
		}
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
	Pos tPos = m_tPos - m_tSize * m_tPivot;
	tPos -= CAMERA->GetTopLeft();
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
		Pos tHitPoint = static_cast<ColliderPixel*>(pDst)->GetHitPoint();
		Rect src = static_cast<ColliderRect*>(pSrc)->GetWorldInfo();

		int bottom = (src.top + src.bottom) / 2;
		if (tHitPoint.y < bottom)
		{
			if (!IsMoveUp())
			{
				SetPos(GetPos().x, GetPos().y - (bottom - tHitPoint.y));
				JumpEnd();
			}
		}
		OnGround();
	}
}

void Player::Fire()
{
	Object* pBullet = Object::CreateCloneObject("Bullet", "PlayerBullet",m_pScene->GetSceneType(), m_pLayer);

	pBullet->AddCollisionFunction("BulletBody", CS_ENTER, (Bullet*) pBullet, &Bullet::Hit);

	Pos tPos;
	tPos.x = GetRight() + pBullet->GetPivot().x * pBullet->GetSize().x;
	tPos.y = GetCenter().y;

	pBullet->SetPos(tPos.x, tPos.y - pBullet->GetSize().y / 2.f);

	SAFE_RELEASE(pBullet);
}
#include <cassert>
#include "Player.h"
#include "Bullet.h"
#include "../../Math.h"
#include "../../Scene/Scene.h"
#include "../../Core/Input.h"
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

void Player::InitRightAnimation()
{
	assert(m_pAnimation != nullptr);
	wstring basePath = L"Player/AmongUs/WalkRight/";
	vector<wstring> vecFileName;
	for (int i = 1; i <= 6; ++i)
	{
		wstringstream ss;
		ss << basePath << L"WalkRight_" << setfill(L'0') << setw(4) << i << "_80x100.bmp";
		vecFileName.push_back(ss.str());
	}
	for (int i = 12; i >= 7; --i)
	{
		wstringstream ss;
		ss << basePath << L"WalkRight_" << setfill(L'0') << setw(4) << i << "_80x100.bmp";
		vecFileName.push_back(ss.str());
	}
	for (int i = 7; i <= 12; ++i)
	{
		wstringstream ss;
		ss << basePath << L"WalkRight_" << setfill(L'0') << setw(4) << i << "_80x100.bmp";
		vecFileName.push_back(ss.str());
	}
	for (int i = 6; i >= 1; --i)
	{
		wstringstream ss;
		ss << basePath << L"WalkRight_" << setfill(L'0') << setw(4) << i << "_80x100.bmp";
		vecFileName.push_back(ss.str());
	}
	AddAnimationClip("WalkRight",
		AT_FRAME, AO_LOOP,
		0.0f, 0.8f,
		24, 1,
		0, 0,
		24, 1,
		0.f, "WalkRight_Anim", vecFileName);
	SetClipColorKey("WalkRight", 255, 255, 255);
}

void Player::SetState(PlayerState state)
{
	m_eState = state;
	if (!m_pAnimation) return;
	switch (state)
	{
	case PlayerState::IDLE_LEFT:
		m_pAnimation->ChangeClip("IdleLeft");
		break;
	case PlayerState::IDLE_RIGHT:
		m_pAnimation->ChangeClip("IdleRight");
		break;
	case PlayerState::WALK_LEFT:
		m_pAnimation->ChangeClip("WalkLeft");
		break;
	case PlayerState::WALK_RIGHT:
		m_pAnimation->ChangeClip("WalkRight");
		break;
	}
}

bool Player::Init()
{
	SetPos(40.0f, 50.0f);
	SetSize(80.0f, 100.0f);
	SetPivot(0.5f, 0.5f);
	SetSpeed(400.0f);

	SetTexture("PlayerIdle", L"Player/AmongUs/IdleRight.bmp");
	SetColorKey(255, 255, 255);
	m_iHP = 1000;

	// 중력을 적용한다.
	SetPhysics(false);

	// 점프할 힘을 설정한다.
	SetForce(500.f);

	SetEnableAnimation(true);
	Animation *pAnim = CreateAnimation("PlayerAnimation");
	AddAnimationClip("WalkLeft", 
						AT_ATLAS, AO_LOOP,
						0.0f, 0.8f,
						6, 4,
						0, 0,
						6, 4,
						0.f, "WalkLeft_Anim",
						L"Player/AmongUs/WalkLeft/WalkLeft_Anim.bmp");
	SetClipColorKey("WalkLeft", 255, 255, 255);
	InitRightAnimation();
	AddAnimationClip("IdleLeft",
					AT_FRAME, AO_LOOP,
					0.0f, 100.0f,
					1, 0,
					0, 0,
					1, 0,
					0.f, "IdleLeft_Anim",
					L"Player/AmongUs/IdleLeft.bmp");
	SetClipColorKey("IdleLeft", 255, 255, 255);
	AddAnimationClip("IdleRight",
					AT_FRAME, AO_LOOP,
					0.0f, 100.0f,
					1, 0,
					0, 0,
					1, 0,
					0.f, "IdleRight_Anim",
					L"Player/AmongUs/IdleRight.bmp");
	SetClipColorKey("IdleRight", 255, 255, 255);
	SAFE_RELEASE(pAnim);

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
	if (KEYPRESS("MoveBack"))
	{
		MoveYFromSpeed(dt, MD_FRONT);
	}
	if (KEYPRESS("MoveFront"))
	{
		// Jump();
		 MoveYFromSpeed(dt, MD_BACK);
	}
	if (KEYPRESS("MoveLeft"))
	{
		MoveXFromSpeed(dt, MD_BACK);
		SetState(PlayerState::WALK_LEFT);
	}
	if (KEYPRESS("MoveRight"))
	{
		MoveXFromSpeed(dt, MD_FRONT);
		SetState(PlayerState::WALK_RIGHT);
	}
	//if (KEYDOWN("Fire"))
	//{
	//	Fire();
	//}
	//if (KEYDOWN("Skill1"))
	//{
	//	MessageBox(NULL, L"Skill1", L"Skill1", MB_OK);
	//}
	if (!KEYPRESS("MoveRight") && !KEYPRESS("MoveLeft"))
	{
		auto curState = GetState();
		if(curState == PlayerState::WALK_LEFT) SetState(PlayerState::IDLE_LEFT);
		else if(curState == PlayerState::WALK_RIGHT) SetState(PlayerState::IDLE_RIGHT);
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
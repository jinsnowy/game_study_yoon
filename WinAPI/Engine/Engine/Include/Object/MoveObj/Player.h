#pragma once
#include "../../framework.h"
#include "MovableObject.h"

class Player : public MovableObject
{
	friend class PlayerTool;
	friend class GameScene;
	friend class SceneManager;
public:
	enum PlayerState
	{
		IDLE_LEFT,
		IDLE_RIGHT,
		IDLE_DOWN,
		IDLE_UP,
		WALK_LEFT,
		WALK_RIGHT,
		WALK_DOWN,
		WALK_UP,
		TOOL_LEFT,
		TOOL_RIGHT,
		TOOL_DOWN,
		TOOL_UP,
	};
private:
	class PlayerTool* m_pTool = nullptr;
	PlayerState m_eState = PlayerState::IDLE_RIGHT;
	Player();
	Player(const Player& obj);
	~Player();
private:
	int m_iHP = 0;
public:
	void InitTexture();
	void InitAnimation();
	PlayerState GetState() const { return m_eState; }
public:
	virtual void LateInit();
	virtual void StateTransit(int iNext);
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual Player* Clone();
public:
	void Hit(class Collider* pSrc, class Collider* pDst, float dt);
	void HitPixel(class Collider* pSrc, class Collider* pDst, float dt);
private:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
private:
	inline Pos GetCenterPos() const
	{
		return Pos(GetPos().x + GetSize().x / 2, GetPos().y);
	}
};


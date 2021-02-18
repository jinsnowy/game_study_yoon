#pragma once
#include "CObject.h"
class CMoveObj :
    public CObject
{
protected:
    CMoveObj();
	CMoveObj(const CMoveObj& obj);
    virtual ~CMoveObj();

private:
	float m_fAngle;
	float m_fSpeed;

public:

	void SetAngle(float angle) { m_fAngle = angle; }
	void SetSpeed(float speed) { m_fSpeed = speed; }
	float GetAngle() const { return m_fAngle; }
	float GetSpeed() const { return m_fSpeed; }


	void Move(float x, float y);
	void Move(float x, float y, float fDeltaTime);
	void Move(const POSITION& tMove, float fDeltaTime);
	
	void MoveXFromSpeed(float fDeltaTime, MOVE_DIR dir);
	void MoveYFromSpeed(float fDeltaTime, MOVE_DIR dir);

	void MoveX(float x);
	void MoveX(float x, float fDeltaTime);
	void MoveY(float y);
	void MoveY(float y, float fDeltaTime);
	
	void MoveAngle(float fSpeed);
	void MoveAngle(float fSpeed, float fTime);
public:
	virtual bool Init() = 0;
	virtual void Input(float fDeltaTime);
	virtual int Update(float fDeltaTime);
	virtual int LateUpdate(float fDeletaTime);
	virtual void Collision(float fDeltaTime);
	virtual void Render(HDC hDC, float fDeltaTime);

};


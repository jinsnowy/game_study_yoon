#pragma once
#include "UI.h"

enum BUTTON_STATE
{
	BS_NONE,
	BS_MOUSEON,
	BS_CLICK
};

class UIButton : public UI
{
private:
	friend class Object;
	friend class Scene;

private:
	UIButton();
	UIButton(const UIButton& ui);
	virtual ~UIButton();

private:
	Pos m_tMouseOnImageOffset;
	Pos m_tMouseOutImageOffset;
	function<void(float fTime)> m_BtnCallback;
	bool	m_bEnableCallback;
	BUTTON_STATE m_eState;
public:
	void SetMouseOnImageOffset(float x, float y) { m_tMouseOnImageOffset = Pos(x, y); }
	void SetMouseOnImageOffset(const Pos& offset) { m_tMouseOnImageOffset = offset; }
	void SetMouseOutImageOffset(float x, float y) { m_tMouseOutImageOffset = Pos(x, y); }
	void SetMouseOutImageOffset(const Pos& offset) { m_tMouseOutImageOffset = offset; }
	template<typename T>
	void SetCallback(T* pObj, void(T::* pFunc)(float))
	{
		m_BtnCallback = bind(pFunc, pObj, placeholders::_1);
		m_bEnableCallback = true;
	}
	void SetCallback(void (*pFunc)(float))
	{
		m_BtnCallback = bind(pFunc, placeholders::_1);
		m_bEnableCallback = true;
	}
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UIButton* Clone();

public:
	void MouseOn(class Collider* pSrc, class Collider* pDst, float dt);
	void MouseOut(class Collider* pSrc, class Collider* pDst, float dt);
};

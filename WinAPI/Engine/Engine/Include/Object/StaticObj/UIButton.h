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
	friend class Layer;
	friend class Scene;
private:
	UIButton();
	UIButton(const UIButton& ui);
	virtual ~UIButton();

private:
	bool m_bUseMouseOnOutImage = false;
	bool m_bUseSound;
	string m_strSoundTag;
	Pos m_tMouseOnImageOffset;
	Pos m_tMouseOutImageOffset;
	function<void(float fTime)> m_BtnCallback;
	bool	m_bEnableCallback;
	BUTTON_STATE m_eState;
public:
	void SetMouseOnOutImage(bool bSet) { m_bUseMouseOnOutImage = bSet; }
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
	template<typename T>
	void SetCallbackByType(T* pObj, UIButton* btn, int type, void(T::* pFunc)(UIButton*, int, float))
	{
		m_BtnCallback = bind(pFunc, pObj, btn, type, placeholders::_1);
		m_bEnableCallback = true;
	}
	void SetCallback(void (*pFunc)(float))
	{
		m_BtnCallback = bind(pFunc, placeholders::_1);
		m_bEnableCallback = true;
	}
	void SetSoundTag(const string& strTag)
	{
		m_bUseSound = false;
		if (strTag.size())
		{
			m_bUseSound = true;
			m_strSoundTag = strTag;
		}
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


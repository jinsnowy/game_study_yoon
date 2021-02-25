#pragma once
#include "UI.h"
class UIButton : public UI
{
private:
	friend class Object;
	friend class Scene;
private:
	UIButton();
	UIButton(const UIButton& ui);
	virtual ~UIButton();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UIButton* Clone();
};


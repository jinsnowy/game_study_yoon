#include "UIButton.h"
#include "../Collider/ColliderRect.h"

UIButton::UIButton()
{

}

UIButton::UIButton(const UIButton& ui)
	: UI(ui)
{
}

UIButton::~UIButton()
{
}

bool UIButton::Init()
{
	ColliderRect* pColl = AddCollider<ColliderRect>("ButtonBody");
	SAFE_RELEASE(pColl);
	return true;
}

void UIButton::Input(float dt)
{
	UI::Input(dt);
}

int UIButton::Update(float dt)
{
	UI::Update(dt);
	return 0;
}

int UIButton::LateUpdate(float dt)
{
	UI::LateUpdate(dt);
	return 0;
}

void UIButton::Collision(float dt)
{
	UI::Collision(dt);
}

void UIButton::Draw(HDC hdc, float dt)
{
	UI::Draw(hdc, dt);
}

UIButton* UIButton::Clone()
{
	return new UIButton(*this);
}

#include "Mouse.h"
#include "../framework.h"
#include "../Application/Window.h"
Mouse::Mouse()
    : m_tMove()
{
}

Mouse::Mouse(const Mouse& ui)
    : UI(ui)
{
}

Mouse::~Mouse()
{

}

bool Mouse::Init()
{
    POINT pt;
    // 스크린 좌표 기준으로 나온다.
    GetCursorPos(&pt);
    ScreenToClient(WINDOWHANDLE, &pt);

    ShowCursor(FALSE);

    m_tPos.x = pt.x;
    m_tPos.y = pt.y;
    return true;
}

void Mouse::Input(float dt)
{
    UI::Input(dt);
}

int Mouse::Update(float dt)
{
    UI::Update(dt);

    POINT pt;
    // 스크린 좌표 기준으로 나온다.
    GetCursorPos(&pt);
    ScreenToClient(WINDOWHANDLE, &pt);

    m_tMove.x = pt.x - m_tPos.x;
    m_tMove.y = pt.y - m_tPos.y;

    m_tPos.x = pt.x;
    m_tPos.y = pt.y;
    return 0;
}

int Mouse::LateUpdate(float dt)
{
    UI::LateUpdate(dt);
    return 0;
}

void Mouse::Collision(float dt)
{
    UI::Collision(dt);
}

void Mouse::Draw(HDC hdc, float dt)
{
    UI::Draw(hdc, dt);
}

Mouse* Mouse::Clone()
{
    return new Mouse(*this);
}

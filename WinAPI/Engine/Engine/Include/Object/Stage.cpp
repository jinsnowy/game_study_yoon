#include "Stage.h"
#include "../Resources/Texture.h"
#include "../framework.h"
#include "../Core/Camera.h"
#include "../Application/Window.h"

Stage::Stage()
{
}

Stage::Stage(const Stage& obj)
    :
    StaticObject(obj)
{
}

Stage::~Stage()
{
}

bool Stage::Init()
{
    SetPos(0.0f, 0.0f);
    SetSize(1920.f, 1080.f);
    SetPivot(0.0f, 0.0f);

    SetTexture("Stage2", L"MaskStage2.bmp");

    return true;
}

void Stage::Input(float dt)
{
    StaticObject::Input(dt);
}

int Stage::Update(float dt)
{
    StaticObject::Update(dt);
    return 0;
}

int Stage::LateUpdate(float dt)
{
    StaticObject::LateUpdate(dt);
    return 0;
}

void Stage::Collision(float dt)
{
    StaticObject::Collision(dt);
}

void Stage::Draw(HDC hDC, float dt)
{
    // StaticObject::Draw(hDC, dt);
    if (m_pTexture)
    {
        Pos tPos = m_Pos - m_Pivot * m_Size;
        Pos tCamTopLeft = CAMERA.GetTopLeft();
        Pos tPivot = CAMERA.GetPivot();
        BitBlt(hDC, int(tPos.x), int(tPos.y),
            CAMERA.GetClientRS().x, CAMERA.GetClientRS().y,
            m_pTexture->GetDC(),
            int(tCamTopLeft.x), int(tCamTopLeft.y),
            SRCCOPY);
    }
}

Stage* Stage::Clone()
{
    return new Stage(*this);
}

#include "TargetRect.h"
#include "../../Core/Camera.h"
TargetRect::TargetRect()
    :
    m_pTarget(nullptr)
{
}

TargetRect::TargetRect(const TargetRect& obj)
    :
    StaticObject(obj)
{
}

TargetRect::~TargetRect()
{
    SAFE_RELEASE(m_pTarget);
}

bool TargetRect::Init()
{
    return true;
}

void TargetRect::Input(float dt)
{
    StaticObject::Input(dt);
    if (m_pTarget)
    {
        m_tPos += m_pTarget->GetPos();
    }
}

int TargetRect::Update(float dt)
{
    StaticObject::Update(dt);
    return 0;
}

int TargetRect::LateUpdate(float dt)
{
    StaticObject::LateUpdate(dt);
    return 0;
}

void TargetRect::Collision(float dt)
{
    StaticObject::Collision(dt);
}

void TargetRect::Draw(HDC hDC, float dt)
{
    StaticObject::Draw(hDC, dt);
    Pos tPos = GetPos();
    Pos tCamPos = CAMERA->GetTopLeft();
    Size tSize = GetSize();

    int left = int(tPos.x - tCamPos.x);
    int top = int(tPos.y - tCamPos.y);
    int right = left + tSize.x;
    int bottom = top + tSize.y;

    HPEN myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    HPEN OldPen = (HPEN)SelectObject(hDC, myPen);
    HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

    Rectangle(hDC, left, top, right, bottom);

    DeleteObject(SelectObject(hDC, OldPen));
    DeleteObject(SelectObject(hDC, OldBrush));
}

TargetRect* TargetRect::Clone()
{
    return new TargetRect(*this);
}

void TargetRect::Save(FILE* pFile)
{
}

void TargetRect::Load(FILE* pFile)
{
}

void TargetRect::SetTarget(Object* obj)
{
    SAFE_RELEASE(m_pTarget);
    m_pTarget = obj;
    if(m_pTarget)
        m_pTarget->AddRef();
}

#include "Layer.h"
#include "../Object/Object.h"

Layer::Layer()
    :
    m_zOrder(0),
    m_tag(""),
    m_pScene(nullptr)
{

}
Layer::~Layer()
{
    Delete_SharedPtr_VecList(m_ObjList);
}

void Layer::AddObject(Object* pObj)
{
    pObj->SetScene(m_pScene);
    pObj->SetLayer(this);
    
    m_ObjList.emplace_back(pObj);
}

void Layer::Input(float dt)
{
    for (auto it = m_ObjList.begin(); it != m_ObjList.end(); it++)
    {
        (*it)->Input(dt);
    }
}

void Layer::Update(float dt)
{
    for (auto it = m_ObjList.begin(); it != m_ObjList.end(); it++)
    {
        (*it)->Update(dt);
    }
}

void Layer::LateUpdate(float dt)
{
    for (auto it = m_ObjList.begin(); it != m_ObjList.end(); it++)
    {
        (*it)->LateUpdate(dt);
    }
}

void Layer::Collision(float dt)
{
    for (auto it = m_ObjList.begin(); it != m_ObjList.end(); it++)
    {
        (*it)->Collision(dt);
    }
}

void Layer::Draw(HDC hdc, float dt)
{
    for (auto it = m_ObjList.begin(); it != m_ObjList.end(); it++)
    {
        (*it)->Draw(hdc, dt);
    }
}

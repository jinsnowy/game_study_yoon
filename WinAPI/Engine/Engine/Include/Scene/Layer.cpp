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
    Safe_Release_VecList(m_ObjList);
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

void Layer::Draw(Graphics& gfx, float dt)
{
    for (auto it = m_ObjList.begin(); it != m_ObjList.end(); it++)
    {
        (*it)->Draw(gfx, dt);
    }
}

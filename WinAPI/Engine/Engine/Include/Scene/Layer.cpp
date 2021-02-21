#include "Layer.h"
#include "../Object/Object.h"
#include "../Collider/CollisionManager.h"

Layer::Layer()
    :
    m_bEnable(true),
    m_bLife(true),
    m_zOrder(0),
    m_tag(""),
    m_pScene(nullptr)
{

}
Layer::~Layer()
{
    auto iterEnd = m_ObjList.end();
    for (auto it = m_ObjList.begin(); it != iterEnd; it++)
    {
        Object::EraseObject(*it);
        SAFE_RELEASE((*it));
    }
    m_ObjList.clear();
}

void Layer::AddObject(Object* pObj)
{
    pObj->SetScene(m_pScene);
    pObj->SetLayer(this);
    pObj->AddRef();
    
    m_ObjList.emplace_back(pObj);
}

void Layer::Input(float dt)
{
    auto iterEnd = m_ObjList.end();
    for (auto it = m_ObjList.begin(); it != iterEnd;)
    {
        if (!(*it)->GetEnable())
        {
            ++it;
            continue;
        }

        (*it)->Input(dt);

        if (!(*it)->GetLife())
        {
            Object::EraseObject(*it);
            SAFE_RELEASE((*it));
            it = m_ObjList.erase(it);
            iterEnd = m_ObjList.end();
        }
        else ++it;
    }
}

void Layer::Update(float dt)
{
    auto iterEnd = m_ObjList.end();
    for (auto it = m_ObjList.begin(); it != iterEnd;)
    {
        if (!(*it)->GetEnable())
        {
            ++it;
            continue;
        }

        (*it)->Update(dt);

        if (!(*it)->GetLife())
        {
            Object::EraseObject(*it);
            SAFE_RELEASE((*it));
            it = m_ObjList.erase(it);
            iterEnd = m_ObjList.end();
        }
        else ++it;
    }
}

void Layer::LateUpdate(float dt)
{
    auto iterEnd = m_ObjList.end();
    for (auto it = m_ObjList.begin(); it != iterEnd;)
    {
        if (!(*it)->GetEnable())
        {
            ++it;
            continue;
        }

        (*it)->LateUpdate(dt);

        if (!(*it)->GetLife())
        {
            Object::EraseObject(*it);
            SAFE_RELEASE((*it));
            it = m_ObjList.erase(it);
            iterEnd = m_ObjList.end();
        }
        else ++it;
    }
}

void Layer::Collision(float dt)
{
    auto iterEnd = m_ObjList.end();
    for (auto it = m_ObjList.begin(); it != iterEnd;)
    {
        if (!(*it)->GetEnable())
        {
            ++it;
            continue;
        }

        if (!(*it)->GetLife())
        {
            Object::EraseObject(*it);
            SAFE_RELEASE((*it));
            it = m_ObjList.erase(it);
            iterEnd = m_ObjList.end();
        }
        else 
        {
            COLLISION_MANAGER.AddObject(*it);
            ++it; 
        }
    }
}

void Layer::Draw(HDC hdc, float dt)
{
    auto iterEnd = m_ObjList.end();
    for (auto it = m_ObjList.begin(); it != iterEnd;)
    {
        if (!(*it)->GetEnable())
        {
            ++it;
            continue;
        }

        (*it)->Draw(hdc, dt);

        if (!(*it)->GetLife())
        {
            Object::EraseObject(*it);
            SAFE_RELEASE((*it));
            it = m_ObjList.erase(it);
            iterEnd = m_ObjList.end();
        }
        else ++it;
    }
}

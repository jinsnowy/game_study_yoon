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
    list<Object*>::iterator it;
    list<Object*>::iterator iterEnd = m_LayerObjList.end();

    for (it = m_LayerObjList.begin(); it != iterEnd; it++)
    {
        Object::EraseObject(*it);
        SAFE_RELEASE((*it));
    }
    m_LayerObjList.clear();
}

void Layer::AddObject(Object* pObj)
{
    pObj->SetScene(m_pScene);
    pObj->SetLayer(this);
    pObj->AddRef();
    
    m_LayerObjList.push_back(pObj);
}

void Layer::Input(float dt)
{
    auto iterEnd = m_LayerObjList.end();
    for (auto it = m_LayerObjList.begin(); it != iterEnd;)
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
            it = m_LayerObjList.erase(it);
            iterEnd = m_LayerObjList.end();
        }
        else ++it;
    }
}

void Layer::Update(float dt)
{
    auto iterEnd = m_LayerObjList.end();
    for (auto it = m_LayerObjList.begin(); it != iterEnd;)
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
            it = m_LayerObjList.erase(it);
            iterEnd = m_LayerObjList.end();
        }
        else ++it;
    }
}

void Layer::LateUpdate(float dt)
{
    auto iterEnd = m_LayerObjList.end();
    for (auto it = m_LayerObjList.begin(); it != iterEnd;)
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
            it = m_LayerObjList.erase(it);
            iterEnd = m_LayerObjList.end();
        }
        else ++it;
    }
}

void Layer::Collision(float dt)
{
    auto iterEnd = m_LayerObjList.end();
    for (auto it = m_LayerObjList.begin(); it != iterEnd;)
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
            it = m_LayerObjList.erase(it);
            iterEnd = m_LayerObjList.end();
        }
        else 
        {
            COLLISION_MANAGER->AddObject(*it);
            ++it; 
        }
    }
}

void Layer::Draw(HDC hdc, float dt)
{
    auto iterEnd = m_LayerObjList.end();
    for (auto it = m_LayerObjList.begin(); it != iterEnd;)
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
            it = m_LayerObjList.erase(it);
            iterEnd = m_LayerObjList.end();
        }
        else ++it;
    }
}

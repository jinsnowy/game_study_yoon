#include "CollisionManager.h"
#include "../Object/Object.h"
#include "Collider.h"

DEFINITION_SINGLE(CollisionManager);

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::Init()
{
    return true;
}

void CollisionManager::AddObject(Object* pObj)
{
    if (pObj->CheckCollider())
    {
        m_CollisionList.push_back(pObj);
    }
}

void CollisionManager::Collision(float dt)
{
    if (m_CollisionList.size() < 2)
    {
        m_CollisionList.clear();
        return;
    }

    // ������Ʈ �� �浹 ó���� �Ѵ�.
    
    
    
    
    
    
    m_CollisionList.clear();
}

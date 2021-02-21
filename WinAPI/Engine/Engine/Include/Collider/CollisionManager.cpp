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

    // 오브젝트 간 충돌 처리를 한다.
    
    
    
    
    
    
    m_CollisionList.clear();
}

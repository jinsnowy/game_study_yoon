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
        m_CollisionObjList.push_back(pObj);
    }
}

void CollisionManager::Collision(float dt)
{
    if (m_CollisionObjList.size() < 2)
    {
        m_CollisionObjList.clear();
        return;
    }

    // ������Ʈ �� �浹 ó���� �Ѵ�.
    list<Object*>::iterator iter;
    list<Object*>::iterator iterEnd = m_CollisionObjList.end();
    --iterEnd;
   
    for (iter = m_CollisionObjList.begin(); iter != iterEnd; ++iter)
    {
        list<Object*>::iterator jter = iter;
        ++jter;
        list<Object*>::iterator jterEnd = m_CollisionObjList.end();
        for (; jter != jterEnd; ++jter)
        {
            CheckCollision(*iter, *jter, dt);
        }
    }
    
    m_CollisionObjList.clear();
}

bool CollisionManager::CheckCollision(Object* pSrc, Object* pDst, float dt)
{
    const list<Collider*>* pSrcList = pSrc->GetColliderList();
    const list<Collider*>* pDstList = pDst->GetColliderList();

    list<Collider*>::const_iterator iterSrc;
    list<Collider*>::const_iterator iterSrcEnd = pSrcList->end();

    list<Collider*>::const_iterator iterDst;
    list<Collider*>::const_iterator iterDstEnd = pDstList->end();

    bool bCollision = false;
    for (iterSrc = pSrcList->begin(); iterSrc != iterSrcEnd; ++iterSrc)
    {
        for (iterDst = pDstList->begin(); iterDst != iterDstEnd; ++iterDst)
        {
            // �浹ü�� ���� �浹�ߴ°�
            if ((*iterSrc)->CheckCollision(*iterDst))
            {
                bCollision = true;

                // ��Ʈ ����Ʈ ����
                ((*iterDst))->SetHitPoint((*iterSrc)->GetHitPoint());

                // �浹��Ͽ��� ������ �浹�� ���� ���ٸ�
                // ó�� �� �浹�Ǿ��ٴ� �ǹ�
                if (!(*iterSrc)->CheckCollisionList(*iterDst))
                {
                    // ���� ������ �浹 ������� �߰��Ѵ�.
                    (*iterSrc)->AddCollider(*iterDst);
                    (*iterDst)->AddCollider(*iterSrc);

                    (*iterSrc)->CallFunction(CS_ENTER, *iterDst, dt);
                    (*iterDst)->CallFunction(CS_ENTER, *iterSrc, dt);
                }
                // ���� �浹�� ���� �ִٸ� ��� �浹 ������ ���̴�.
                else
                {
                    (*iterSrc)->CallFunction(CS_STAY, *iterDst, dt);
                    (*iterDst)->CallFunction(CS_STAY, *iterSrc, dt);
                }
            }
            // ���� �浹���� �ʾҴٸ�,
            // ���� �浹�� �ȵ� ���¿��� ������ �浹�� �ǰ� �־�
            // �ٸ� ���� �� �浹���¿��� �������ٴ� �ǹ��̴�.
            else if ((*iterSrc)->CheckCollisionList(*iterDst))
            {
                // ���� �浹�� �ȵǹǷ� �浹��Ͽ��� �����ش�.
                (*iterSrc)->EraseCollisionList(*iterDst);
                (*iterDst)->EraseCollisionList(*iterSrc);

                (*iterSrc)->CallFunction(CS_LEAVE, *iterDst, dt);
                (*iterDst)->CallFunction(CS_LEAVE, *iterSrc, dt);
            }
        }
    }
    return bCollision;
}

#include "Object.h"
#include "../Scene/Layer.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "../Core/Camera.h"
#include "../Collider/Collider.h"

list<Object*> Object::m_ObjList;

Object::Object() :
    m_pTexture(nullptr),
    m_pScene(nullptr),
    m_pLayer(nullptr),
    m_Pos(0, 0),
    m_Pivot(0, 0),
    m_Size(0, 0),
    m_blsPhysics(false),
    m_fGravityTime(0.f)
{
}

Object::Object(const Object& obj)
{
    *this = obj;

    m_fGravityTime = 0.f;

    if (m_pTexture)
        m_pTexture->AddRef();

    m_ColliderList.clear();

    // 충돌체 복사
    list<Collider*>::const_iterator iter;
    list<Collider*>::const_iterator iterEnd = obj.m_ColliderList.end();
    for (iter = obj.m_ColliderList.begin(); iter != iterEnd; iter++)
    {
        Collider* pColl = (*iter)->Clone();

        pColl->SetObj(this);

        m_ColliderList.push_back(pColl);
    }
}

Object::~Object()
{
    SAFE_RELEASE(m_pTexture);
    Safe_Release_VecList(m_ColliderList);
}

// ----------- Add and Delete Object from Object List
void Object::AddObject(Object* pObj)
{
    pObj->AddRef();
    m_ObjList.emplace_back(pObj);
}

Object* Object::FindObject(const string& tag)
{
    auto iterEnd = m_ObjList.end();
    for (auto it = m_ObjList.begin(); it != iterEnd; ++it)
    {
        if ((*it)->GetTag() == tag)
        {
            return *it;
        }
    }
    return nullptr;
}

void Object::EraseObject(Object* pObj)
{
    auto iterEnd = m_ObjList.end();
    for (auto it = m_ObjList.begin(); it != iterEnd; ++it)
    {
        if (*it == pObj)
        {
            SAFE_RELEASE((*it));
            it = m_ObjList.erase(it);
            return;
        }
    }
}

void Object::EraseObject(const string& tag)
{
    auto iterEnd = m_ObjList.end();
    for (auto it = m_ObjList.begin(); it != iterEnd; ++it)
    {
        if ((*it)->GetTag() == tag)
        {
            SAFE_RELEASE((*it));
            it = m_ObjList.erase(it);
            return;
        }
    }
}

void Object::EraseAllObjects()
{
    Safe_Release_VecList(m_ObjList);
}
// --------------------------

// ----------------------- Texture
void Object::SetTexture(Texture* pTexture)
{
    SAFE_RELEASE(m_pTexture);
    m_pTexture = pTexture;

    if (pTexture)
        pTexture->AddRef();
}

void Object::SetTexture(const string& strKey, const wchar_t* pFileName, const string& strPathKey)
{
    SAFE_RELEASE(m_pTexture);
    m_pTexture = RESOURCE_MANAGER.LoadTexture(strKey, pFileName, strPathKey);
}
// ------------------------

bool Object::Init()
{
    return false;
}

void Object::Input(float dt)
{
}

int Object::Update(float dt)
{

    list<Collider*>::iterator iter;
    list<Collider*>::iterator iterEnd = m_ColliderList.end();

    for (iter = m_ColliderList.begin(); iter != iterEnd;)
    {
        if (!(*iter)->GetEnable())
        {
            ++iter;
            continue;
        }

        (*iter)->Update(dt);

        if (!(*iter)->GetLife())
        {
            SAFE_RELEASE((*iter));
            iter = m_ColliderList.erase(iter);
            iterEnd = m_ColliderList.end();
        }
        else ++iter;
    }
  
    return 0;
}

int Object::LateUpdate(float dt)
{
    list<Collider*>::iterator iter;
    list<Collider*>::iterator iterEnd = m_ColliderList.end();

    for (iter = m_ColliderList.begin(); iter != iterEnd;)
    {
        if (!(*iter)->GetEnable())
        {
            ++iter;
            continue;
        }

        (*iter)->LateUpdate(dt);

        if (!(*iter)->GetLife())
        {
            SAFE_RELEASE((*iter));
            iter = m_ColliderList.erase(iter);
            iterEnd = m_ColliderList.end();
        }
        else ++iter;
    }
    return 0;
}

void Object::Collision(float dt)
{
}

void Object::Draw(HDC hdc, float dt)
{
    if (m_pTexture)
    {
        Pos tPos = m_Pos - m_Size * m_Pivot;
        tPos -= CAMERA.GetTopLeft();
        if (m_pTexture->GetColorKeyEnable())
        {
            TransparentBlt(hdc, int(tPos.x), int(tPos.y), int(m_Size.x), int(m_Size.y),
                m_pTexture->GetDC(), 0, 0,
                int(m_Size.x), int(m_Size.y),
                m_pTexture->GetColorKey());
        }
        else 
        {
            BitBlt(hdc, int(tPos.x), int(tPos.y), int(m_Size.x), int(m_Size.y), m_pTexture->GetDC(), 0, 0, SRCCOPY);
        }
    }

    list<Collider*>::iterator iter;
    list<Collider*>::iterator iterEnd = m_ColliderList.end();

    for (iter = m_ColliderList.begin(); iter != iterEnd;)
    {
        if (!(*iter)->GetEnable())
        {
            ++iter;
            continue;
        }

        (*iter)->Draw(hdc, dt);

        if (!(*iter)->GetLife())
        {
            SAFE_RELEASE((*iter));
            iter = m_ColliderList.erase(iter);
            iterEnd = m_ColliderList.end();
        }
        else ++iter;
    }
}

Object* Object::CreateCloneObject(const string& strPrototypeKey, const string& strTag, class Layer* pLayer)
{
    Object* pProto = Scene::FindPrototype(strPrototypeKey);

    if (!pProto)
        return nullptr;

    // 복제한 인스턴스
    Object* pObj = pProto->Clone();
    pObj->SetTag(strTag);

    if (pLayer)
    {
        pLayer->AddObject(pObj);
    }

    AddObject(pObj);

    return pObj;
}

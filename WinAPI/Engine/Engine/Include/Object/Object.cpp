#include "Object.h"
#include "../Scene/Layer.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"

list<Object*> Object::m_ObjList;

Object::Object() :
    m_pTexture(nullptr),
    m_pScene(nullptr),
    m_pLayer(nullptr),
    m_Pos(0, 0),
    m_Pivot(0, 0),
    m_Size(0, 0)
{
}

Object::Object(const Object& obj)
{
    *this = obj;

    if (m_pTexture)
        m_pTexture->AddRef();
}

Object::~Object()
{
    SAFE_RELEASE(m_pTexture);
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

void Object::SetTexture(const string& strKey, const char* pFileName, const string& strPathKey)
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
    return 0;
}

int Object::LateUpdate(float dt)
{
    return 0;
}

void Object::Collision(float dt)
{
}

void Object::Draw(HDC hdc, float dt)
{
    if (m_pTexture)
    {
        // (x,y) -> (cx,cy) = (x+w,y+h) / 어디서 부터 출력 (x1,y1) 
        BitBlt(hdc, int(m_Pos.x), int(m_Pos.y), int(m_Size.x), int(m_Size.y), m_pTexture->GetDC(), 0, 0, SRCCOPY);
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

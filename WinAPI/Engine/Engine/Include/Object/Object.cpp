#include "Object.h"
#include "../Scene/Layer.h"

list<Object*> Object::m_ObjList;
unordered_map<string, Object*> Object::m_mapProtoType;

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

void Object::ErasePrototype(const string& strPrototypeKey)
{
    auto it = m_mapProtoType.find(strPrototypeKey);
    if (!it->second)
        return;

    SAFE_RELEASE(it->second);
    m_mapProtoType.erase(it);
}

void Object::EraseAllObjects()
{
    Safe_Release_VecList(m_ObjList);
}

void Object::EraseAllPrototypes()
{
    Safe_Delete_Map(m_mapProtoType);
}

Object::Object():
    m_pScene(nullptr),
    m_pLayer(nullptr),
    m_Pos(0,0),
    m_Pivot(0,0),
    m_Size(0,0)
{
}

Object::~Object()
{
}

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
}

Object* Object::CreateCloneObject(const string& strPrototypeKey, const string& strTag, class Layer* pLayer)
{
    Object* pProto = FindPrototype(strPrototypeKey);
    if (!pProto)
        return nullptr;

     Object* pObj = pProto->Clone();
     pObj->SetTag(strTag);

     if (pLayer)
     {
         SAFE_RELEASE(pObj);
         pLayer->AddObject(pObj);
     }

     AddObject(pObj);

     return pObj;
}

Object* Object::FindPrototype(const string& strkey)
{
    auto it = m_mapProtoType.find(strkey);
    if (it == m_mapProtoType.end())
        return nullptr;
    return it->second;
}

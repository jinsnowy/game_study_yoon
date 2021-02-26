#include <cassert>
#include "Object.h"
#include "../Scene/Layer.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "../Core/Camera.h"
#include "../Collider/Collider.h"
#include "../Animation/Animation.h"

list<Object*> Object::m_ObjList;

Object::Object() :
    m_pTexture(nullptr),
    m_pAnimation(nullptr),
    m_pScene(nullptr),
    m_pLayer(nullptr),
    m_tPos(0, 0),
    m_tPivot(0, 0),
    m_tSize(0, 0),
    m_tImageOffset(0, 0),
    m_blsPhysics(false),
    m_bEnableAnimation(false),
    m_fGravityTime(0.f)
{
}

Object::Object(const Object& obj)
{
    *this = obj;

    if (obj.m_pAnimation)
        m_pAnimation = obj.m_pAnimation->Clone();

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
    SAFE_RELEASE(m_pAnimation);
    SAFE_RELEASE(m_pTexture);
    Safe_Release_VecList(m_ColliderList);
}

// ----------- Add and Delete Object from Object List
void Object::AddObject(Object* pObj)
{
    pObj->AddRef();
    m_ObjList.push_back(pObj);
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
    list<Object*>::iterator it;
    list<Object*>::iterator iterEnd = m_ObjList.end();

    const auto ptr = &m_ObjList;
    for (it = m_ObjList.begin(); it != iterEnd; ++it)
    {
        if (*it == pObj)
        {
            SAFE_RELEASE((*it));
            m_ObjList.erase(it);
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

void Object::SetClipColorKey(const string& strName, unsigned char r, unsigned char g, unsigned char b)
{
    assert(m_pAnimation != nullptr);
    m_pAnimation->SetClipColorKey(strName, r, g, b);
}

Animation* Object::CreateAnimation(const string& strTag)
{
    SAFE_RELEASE(m_pAnimation);

    m_pAnimation = new Animation;

    m_pAnimation->SetTag(strTag);
    m_pAnimation->SetObj(this);

    if (!m_pAnimation->Init())
    {
        SAFE_RELEASE(m_pAnimation);
        return nullptr;
    }

    m_pAnimation->AddRef();

    return m_pAnimation;
}
bool Object::AddAnimationClip(const string& strName,
                                ANIMATION_TYPE eType, ANIMATION_OPTION eOption,
                                float fAnimationTime, float fAnimationLimitTime,
                                int iFrameMaxX, int iFrameMaxY,
                                int iStartX, int iStartY,
                                int iLengthX, int iLengthY,
                                float fOptionLimitTime,
                                const string& strTexKey,
                                const wchar_t* pFileName,
                                const string& strPathKey)
{
    if (!m_pAnimation)
        return false;

    m_pAnimation->AddClip(strName,
                        eType, eOption, fAnimationTime,
                        fAnimationLimitTime,
                        iFrameMaxX, iFrameMaxY,
                        iStartX, iStartY,
                        iLengthX, iLengthY,
                        fOptionLimitTime,
                        strTexKey,
                        pFileName, strPathKey);

    return true;
}
bool Object::AddAnimationClip(const string& strName,
                    ANIMATION_TYPE eType, ANIMATION_OPTION eOption,
                    float fAnimationTime, float fAnimationLimitTime,
                    int iFrameMaxX, int iFrameMaxY,
                    int iStartX, int iStartY,
                    int iLengthX, int iLengthY,
                    float fOptionLimitTime,
                    const string& strTexKey,
                    const vector<wstring>& vecFileName, const string& strPathKey)
{
    if (!m_pAnimation)
        return false;

    m_pAnimation->AddClip(strName,
                        eType, eOption, fAnimationTime,
                        fAnimationLimitTime,
                        iFrameMaxX, iFrameMaxY,
                        iStartX, iStartY,
                        iLengthX, iLengthY,
                        fOptionLimitTime,
                        strTexKey,
                        vecFileName, strPathKey);

    return true;
}
Collider* Object::GetCollider(const string& strTag)
{
    list<Collider*>::iterator iter;
    list<Collider*>::iterator iterEnd = m_ColliderList.end();

    for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
    {
        if ((*iter)->GetTag() == strTag)
        {
            (*iter)->AddRef();
            return *iter;
        }
    }
    return nullptr;
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
    m_pTexture = RESOURCE_MANAGER->LoadTexture(strKey, pFileName, strPathKey);
}

void Object::SetColorKey(unsigned char r, unsigned char g, unsigned char b)
{
    m_pTexture->SetColorKey(r, g, b);
}

void Object::SetAnimationClipColorKey(const string& strClip, unsigned char r, unsigned char g, unsigned char b)
{
    if (m_pAnimation)
    {
        m_pAnimation->SetClipColorKey(strClip, r, g, b);
    }
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
  
    if (m_pAnimation && m_bEnableAnimation)
    {
        m_pAnimation->Update(dt);
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
        Pos tPos = m_tPos - m_tSize * m_tPivot;
        tPos -= CAMERA->GetTopLeft();

        Pos tImagePos = m_tImageOffset;
        if (m_pAnimation && m_bEnableAnimation)
        {
            AnimationClip* pClip = m_pAnimation->GetCurrentClip();

            switch(pClip->eType)
            {
            case AT_FRAME:
                SetTexture(pClip->vecTexture[pClip->iFrameX]);
                break;
            case AT_ATLAS:
                tImagePos.x = pClip->iFrameX * m_tSize.x + m_tImageOffset.x;
                tImagePos.y = pClip->iFrameY * m_tSize.y + m_tImageOffset.y;
                break;
            }
        }

        if (m_pTexture->GetColorKeyEnable())
        {
            TransparentBlt(hdc, int(tPos.x), int(tPos.y), int(m_tSize.x), int(m_tSize.y),
                                m_pTexture->GetDC(), int(tImagePos.x), int(tImagePos.y),
                                int(m_tSize.x), int(m_tSize.y),
                                m_pTexture->GetColorKey());
        }
        else 
        {
            BitBlt(hdc, int(tPos.x), int(tPos.y), int(m_tSize.x), int(m_tSize.y), m_pTexture->GetDC(), 0, 0, SRCCOPY);
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

Object* Object::CreateCloneObject(const string& strPrototypeKey, const string& strTag, SCENE_CREATE sc, class Layer* pLayer)
{
    Object* pProto = Scene::FindPrototype(strPrototypeKey, sc);

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

#include <cassert>
#include "Object.h"
#include "../Scene/Layer.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Resources/PrototypeManager.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "../Core/Camera.h"
#include "../Collider/Collider.h"
#include "../Collider/ColliderPixel.h"
#include "../Collider/ColliderRect.h"
#include "../Collider/ColliderSphere.h"
#include "../Collider/ColliderPoint.h"
#include "../Animation/Animation.h"
#include "../Core/PathManager.h"

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
    m_bHasPrototype(false),
    m_blsPhysics(false),
    m_bEnableAnimation(false),
    m_fGravityTime(0.f)
{
}

Object::Object(const Object& obj)
{
    *this = obj;
    m_Ref = 1;

    if (obj.m_pAnimation)
        m_pAnimation = obj.m_pAnimation->Clone();

    m_fGravityTime = 0.f;

    if (m_pTexture)
        m_pTexture->AddRef();

    m_ColliderList.clear();

    // �浹ü ����
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

Size Object::GetImageSize() const
{
    if (m_pTexture)
        return m_pTexture->GetSize();
    return Size(0, 0);
}

void Object::SetAsTextureSize()
{
    assert(m_pTexture);

    m_tSize.x = float(m_pTexture->GetWidth());
    m_tSize.y = float(m_pTexture->GetHeight());
}

void Object::SetPrototypeTag(const string& prototypeTag)
{
    assert(prototypeTag.size());
    m_strPrototypeTag = prototypeTag;
    m_bHasPrototype = true;
}

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

void Object::DrawImageAt(HDC hdc, const Pos& at)
{
    DrawImageAt(hdc, int(at.x), int(at.y));
}

void Object::DrawImageAt(HDC hdc, int px, int py)
{
    if (m_pTexture)
    {
        Size tSize = GetImageSize();
        int tx = px - m_tPivot.x * tSize.x;
        int ty = py - m_tPivot.y * tSize.y;
        m_pTexture->DrawImageAt(hdc, tx, ty);
    }
}

void Object::DrawImageAtFixedSize(HDC hdc, const Pos& at, int size)
{
    DrawImageAtFixedSize(hdc, int(at.x), int(at.y), size);
}

void Object::DrawImageAtFixedSize(HDC hdc, int px, int py, int size)
{
    if (m_pTexture)
    {
        Size tSize = GetImageSize();
        int tx = px - m_tPivot.x * tSize.x;
        int ty = py - m_tPivot.y * tSize.y;
        m_pTexture->DrawImageAtFixedSize(hdc, tx, ty, size);
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
    Pos tPos = m_tPos - m_tSize * m_tPivot;
    tPos -= CAMERA->GetTopLeft();

    // ī�޶� �ø�
    RESOLUTION tClientRS = CAMERA->GetClientRS();
    if (tPos.x + m_tSize.x < 0 || tPos.x > tClientRS.x || tPos.y + m_tSize.y < 0 || tPos.y > tClientRS.y)
        return;

    if (m_pTexture)
    {
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
            BitBlt(hdc, int(tPos.x), int(tPos.y), int(m_tSize.x), int(m_tSize.y),
                m_pTexture->GetDC(), int(tImagePos.x), int(tImagePos.y), SRCCOPY);
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

void Object::SaveFromFile(FILE* pFile)
{
    assert(pFile);
    Save(pFile);
}

void Object::LoadFromFile(FILE* pFile)
{
    assert(pFile);
    Load(pFile);
}

void Object::SaveFromPath(const char* pFileName, const string& strPathKey)
{
    const char* pPath = PATH_MANAGER->FindPathByMultiByte(DATA_PATH);

    string strFullPath;
    if (pPath)
        strFullPath = pPath;
    strFullPath += pFileName;

    SaveFromFullPath(strFullPath.c_str());
}

void Object::SaveFromFullPath(const char* pFullPath)
{
    FILE* pFile = NULL;

    fopen_s(&pFile, pFullPath, "ab");

    if (pFile)
    {
        Save(pFile);

        fclose(pFile);
    }
}

void Object::Save(FILE* pFile)
{
    // �Ϲ� ������Ʈ ����
    int iLength = m_strTag.length();
    fwrite(&iLength, 4, 1, pFile);
    fwrite(m_strTag.c_str(), 1, iLength, pFile);
    fwrite(&m_blsPhysics, 1, 1, pFile);
    fwrite(&m_tPos, sizeof(m_tPos), 1, pFile);
    fwrite(&m_tSize, sizeof(m_tSize), 1, pFile);
    fwrite(&m_tImageOffset, sizeof(m_tImageOffset), 1, pFile);
    fwrite(&m_tPivot, sizeof(m_tPivot), 1, pFile);

    // Texture ����
    bool bTexture = false;
    if (m_pTexture)
    {
        bTexture = true;
        fwrite(&bTexture, 1, 1, pFile);
        m_pTexture->Save(pFile);
    }
    else {
        fwrite(&bTexture, 1, 1, pFile);
    }

    // �浹ü ���� �����Ѵ�.
    iLength = m_ColliderList.size();

    fwrite(&iLength, 4, 1, pFile);

    list<Collider*>::iterator iter;
    list<Collider*>::iterator iterEnd = m_ColliderList.end();

    for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
    {
        COLLIDER_TYPE eType = (*iter)->GetColliderType();
        fwrite(&eType, 4, 1, pFile);
        (*iter)->Save(pFile);
    }

    // �ִϸ��̼� ����
    bool bAnimation = false;
    if (m_pAnimation)
    {
        bAnimation = true;
        fwrite(&bAnimation, 1, 1, pFile);

        m_pAnimation->Save(pFile);
    }
    else {
        fwrite(&bAnimation, 1, 1, pFile);
    }
}


void Object::LoadFromPath(const char* pFileName, const string& strPathKey)
{
    const char* pPath = PATH_MANAGER->FindPathByMultiByte(DATA_PATH);

    string strFullPath;
    if (pPath)
        strFullPath = pPath;
    strFullPath += pFileName;

    LoadFromFullPath(strFullPath.c_str());
}

void Object::LoadFromFullPath(const char* pFullPath)
{
    FILE* pFile = NULL;

    fopen_s(&pFile, pFullPath, "rb");

    if (pFile)
    {
        Load(pFile);

        fclose(pFile);
    }
}

void Object::Load(FILE* pFile)
{
    // Tag ���� ����
    int iLength = 0;
    char strText[MAX_PATH] = {};

    // Tag ���̸� �о�´�.
    fread(&iLength, 4, 1, pFile);

    // Tag ���ڿ��� �о�´�.
    fread(strText, 1, iLength, pFile);
    strText[iLength] = 0;
    m_strTag = strText;

    fread(&m_blsPhysics, 1, 1, pFile);
    fread(&m_tPos, sizeof(m_tPos), 1, pFile);
    fread(&m_tSize, sizeof(m_tSize), 1, pFile);
    fread(&m_tImageOffset, sizeof(m_tImageOffset), 1, pFile);
    fread(&m_tPivot, sizeof(m_tPivot), 1, pFile);

    // Texture �о�´�
    bool bTexture;
    fread(&bTexture, 1, 1, pFile);
    SAFE_RELEASE(m_pTexture);

    if (bTexture)
    {
        m_pTexture = RESOURCE_MANAGER->LoadTexture(pFile);
    }

    // �浹ü ���� �о�´�.
    iLength = 0;
    fread(&iLength, 4, 1, pFile);

    for (int i = 0; i < iLength; i++)
    {
        COLLIDER_TYPE  eType;
        fread(&eType, 4, 1, pFile);

        Collider* pCollider = nullptr;

        switch (eType)
        {
        case CT_RECT:
            pCollider = AddCollider<ColliderRect>("");
            break;
        case CT_SPHERE:
            pCollider = AddCollider<ColliderSphere>("");
            break;
        case CT_LINE:
            break;
        case CT_POINT:
            pCollider = AddCollider<ColliderPoint>("");
            break;
        case CT_PIXEL:
            pCollider = AddCollider<ColliderPixel>("");
            break;
        }

        pCollider->Load(pFile);

        SAFE_RELEASE(pCollider);
    }

    // �ִϸ��̼� �о�´�.
    bool bAnimation;
    fread(&bAnimation, 1, 1, pFile);
    SAFE_RELEASE(m_pAnimation);

    if (bAnimation)
    {
        m_pAnimation = new Animation;

        m_pAnimation->Init();
        m_pAnimation->Load(pFile);
    }
}

Object* Object::CreateCloneObject(const string& strPrototypeKey, const string& strTag, SCENE_CREATE sc, class Layer* pLayer)
{
    Object* pProto = Scene::FindPrototype(strPrototypeKey, sc);

    if (!pProto)
        return nullptr;

    // ������ �ν��Ͻ�
    Object* pObj = pProto->Clone();
    pObj->SetTag(strTag);

    if (pLayer)
    {
        pLayer->AddObject(pObj);
    }

    AddObject(pObj);

    return pObj;
}

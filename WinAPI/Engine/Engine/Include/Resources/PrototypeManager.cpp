#include "PrototypeManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "../Object/Object.h"
#include "../Scene/Scene.h"
#include "../Scene/Layer.h"
#include "../Core/PathManager.h"
#include "../Object/StaticObj/Tree.h"

DEFINITION_SINGLE(PrototypeManager);

unordered_map<string, Object*> PrototypeManager::m_mapProtoType[PR_END];

PrototypeManager::PrototypeManager()
{
}

PrototypeManager::~PrototypeManager()
{
    for (int i = 0; i < PR_END; ++i)
    {
        Safe_Release_Map(m_mapProtoType[static_cast<PR_TYPE>(i)]);
    }
}

bool PrototypeManager::Init()
{
    Tree* pTree = LoadObject<Tree>();
    RegisterProtoType(PR_PLANT, "Tree1_Prototype", pTree);
    pTree = pTree->Clone();
    pTree->ChangeTreeTexture(1);
    RegisterProtoType(PR_PLANT, "Tree2_Prototype", pTree);
    pTree = pTree->Clone();
    pTree->ChangeTreeTexture(2);
    RegisterProtoType(PR_PLANT, "Tree3_Prototype", pTree);

	return true;
}

void PrototypeManager::RegisterProtoType(PR_TYPE eType, const string& strPrototypeKey, Object* pObj)
{
    if (FindPrototype(eType, strPrototypeKey))
        throw EXCEPT(L"[PrototyeManager] Prototype string already exists");

    assert(pObj != nullptr);

    pObj->SetPrototypeTag(strPrototypeKey);
    m_mapProtoType[eType].insert(make_pair(strPrototypeKey, pObj));
}

Object* PrototypeManager::FindPrototype(const string& strPrototypeKey)
{
    Object* pObj;
    for (int i = 0; i < PR_END; ++i)
    {
        pObj = FindPrototype(static_cast<PR_TYPE>(i), strPrototypeKey);
        if (pObj) return pObj;
    }
    return nullptr;
}

Object* PrototypeManager::FindPrototype(PR_TYPE eType, const string& strPrototypeKeytrTag)
{
	auto found = m_mapProtoType[eType].find(strPrototypeKeytrTag);
	if (found == m_mapProtoType[eType].end())
	{
		return nullptr;
	}
	return found->second;
}

//void PrototypeManager::LoadTileObjectInFolder(PR_TYPE eType, const wchar_t* pBaseFolderName, const string& strPathKey)
//{
//    const wchar_t* pPath = PATH_MANAGER->FindPath(strPathKey);
//    wstring strPath;
//    if (pPath)
//        strPath = pPath;
//
//    strPath += pBaseFolderName;
//    assert(strPath.back() == L'\\' || strPath.back() == L'/');
//
//    const auto extract_key = [](const char* str, int size)
//    {
//        int ed = size - 1;
//        while (str[ed] != L'.') --ed;
//        int st = ed - 1;
//        while (str[st] != L'\\' && str[st] != L'/') st--;
//        return string(str + st + 1, str + ed);
//    };
//
//    Texture* pTex;
//    for (const auto& entry : fs::directory_iterator(strPath))
//    {
//        const wchar_t* path = entry.path().c_str();
//        string strTag = extract_key(GetChar(path), lstrlen(path));
//        string prototypeStrKey = strTag + "_Prototype";
//        Tile* tObject = new Tile;
//        pTex = RESOURCE_MANAGER->LoadTexture(strTag, path, "");
//        pTex->SetColorKey(255, 255, 255);
//
//        tObject->SetPivot(0.f, 1.0f);
//        tObject->SetTexture(pTex);
//        tObject->SetPrototypeTag(prototypeStrKey);
//        tObject->SetTag(strTag);
//        tObject->SetAsTextureSize();
//        SAFE_RELEASE(pTex);
//        RegisterProtoType(eType, prototypeStrKey, tObject);
//    }
//}

Object* PrototypeManager::CreateCloneObject(const string& strPrototypeKey, const string& strTag, Scene* pScene, Layer* pLayer)
{
    assert(strPrototypeKey.ends_with("_Prototype"));
	Object* pProto = FindPrototype(strPrototypeKey);

	if (!pProto)
		return nullptr;

	// 복제한 인스턴스
	Object* pObj = pProto->Clone();
	pObj->SetTag(strTag);

    if (pScene)
    {
        pScene->AddObject(pObj);
    }

	if (pLayer)
	{
        pLayer->AddObject(pObj);
	}

	return pObj;
}
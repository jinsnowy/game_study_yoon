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
#include "PrototypeManager.h"
#include "../Object/Object.h"
#include "../Scene/Layer.h"
DEFINITION_SINGLE(PrototypeManager);
unordered_map<string, Object*> PrototypeManager::m_mapProtoType;

PrototypeManager::PrototypeManager()
{
}

PrototypeManager::~PrototypeManager()
{
	Safe_Release_Map(m_mapProtoType);
}

bool PrototypeManager::Init()
{
	return true;
}

Object* PrototypeManager::FindPrototype(const string& strPrototypeKeytrTag)
{
	auto found = m_mapProtoType.find(strPrototypeKeytrTag);
	if (found == m_mapProtoType.end())
	{
		return nullptr;
	}
	return found->second;
}

Object* PrototypeManager::CreateCloneObject(const string& strPrototypeKey, const string& strTag, class Layer* pLayer)
{
	Object* pProto = FindPrototype(strPrototypeKey);

	if (!pProto)
		return nullptr;

	// 복제한 인스턴스
	Object* pObj = pProto->Clone();
	pObj->SetTag(strTag);

	if (pLayer)
	{
		pLayer->AddObject(pObj);
	}

	Object::AddObject(pObj);

	return pObj;
}
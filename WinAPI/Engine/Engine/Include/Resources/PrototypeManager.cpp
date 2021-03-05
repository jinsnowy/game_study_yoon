#include "PrototypeManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "../Object/Object.h"
#include "../Scene/Layer.h"
#include "../Core/PathManager.h"
#include "../Object/StaticObj/TileObject.h"

DEFINITION_SINGLE(PrototypeManager);

unordered_map<string, Object*> PrototypeManager::m_mapProtoType[OBJ_END];

PrototypeManager::PrototypeManager()
{
}

PrototypeManager::~PrototypeManager()
{
    for (int i = 0; i < OBJ_END; ++i)
    {
        Safe_Release_Map(m_mapProtoType[static_cast<OBJECT_TYPE>(i)]);
    }
}

bool PrototypeManager::Init()
{
    LoadTileObjectInFolder(OBJ_TILE_INNER, L"SV/TileObject/Inner/");
	return true;
}

Object* PrototypeManager::FindPrototype(const string& strPrototypeKey)
{
    Object* pObj;
    for (int i = 0; i < OBJ_END; ++i)
    {
        pObj = FindPrototype(static_cast<OBJECT_TYPE>(i), strPrototypeKey);
        if (pObj) return pObj;
    }
    return nullptr;
}

Object* PrototypeManager::FindPrototype(OBJECT_TYPE eType, const string& strPrototypeKeytrTag)
{
	auto found = m_mapProtoType[eType].find(strPrototypeKeytrTag);
	if (found == m_mapProtoType[eType].end())
	{
		return nullptr;
	}
	return found->second;
}

void PrototypeManager::LoadTileObjectInFolder(OBJECT_TYPE eType, const wchar_t* pBaseFolderName, const string& strPathKey)
{
    const wchar_t* pPath = PATH_MANAGER->FindPath(strPathKey);
    wstring strPath;
    if (pPath)
        strPath = pPath;

    strPath += pBaseFolderName;
    assert(strPath.back() == L'\\' || strPath.back() == L'/');

    const auto extract_key = [](const char* str, int size)
    {
        int ed = size - 1;
        while (str[ed] != L'.') --ed;
        int st = ed - 1;
        while (str[st] != L'\\' && str[st] != L'/') st--;
        return string(str + st + 1, str + ed);
    };

    Texture* pTex;
    for (const auto& entry : fs::directory_iterator(strPath))
    {
        const wchar_t* path = entry.path().c_str();
        string prototypeStrKey = extract_key(GetChar(path), lstrlen(path));
        string strTag = prototypeStrKey + "_Prototype";
        TileObject* tObject = new TileObject;
        pTex = RESOURCE_MANAGER->LoadTexture(prototypeStrKey, path, "");
        pTex->SetColorKey(255, 255, 255);

        tObject->SetPivot(0.f, 1.0f);
        tObject->SetTexture(pTex);
        tObject->SetTag(strTag);
        tObject->SetAsTextureSize();
        SAFE_RELEASE(pTex);
        RegisterProtoType(eType, prototypeStrKey, tObject);
    }
}

Object* PrototypeManager::CreateCloneObject(const string& strPrototypeKey, const string& strTag, class Layer* pLayer)
{
	Object* pProto = FindPrototype(strPrototypeKey);

	if (!pProto)
		return nullptr;

	// ������ �ν��Ͻ�
	Object* pObj = pProto->Clone();
	pObj->SetTag(strTag);

	if (pLayer)
	{
		pLayer->AddObject(pObj);
	}

	Object::AddObject(pObj);

	return pObj;
}
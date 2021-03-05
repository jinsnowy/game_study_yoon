#pragma once
#include "../framework.h"

class Object;
class Layer;
class PrototypeManager
{
	friend class Object;
	friend class UITileSelect;
	DECLARE_SINGLE(PrototypeManager);
private:
	static unordered_map<string, Object*> m_mapProtoType[OBJ_END];
	unordered_map<string, Object*>* GetPrototypes(OBJECT_TYPE eType) const { return &m_mapProtoType[eType]; }
public:
	void LoadTileObjectInFolder(OBJECT_TYPE eType, const wchar_t* pBaseFolderName, const string& strPathKey = TEXTURE_PATH);
	template<typename T>
	static void RegisterProtoType(OBJECT_TYPE eType, const string& strPrototypeKey)
	{
		if (FindPrototype(eType, strPrototypeKey))
			throw EXCEPT(L"[PrototyeManager] Prototype string already exists");
		T* pObj = new T;
		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return;
		}
		m_mapProtoType[eType].insert(make_pair(strPrototypeKey, pObj));
	}
	static void RegisterProtoType(OBJECT_TYPE eType, const string& strPrototypeKey, Object* pObj)
	{
		if (FindPrototype(eType, strPrototypeKey))
			throw EXCEPT(L"[PrototyeManager] Prototype string already exists");

		m_mapProtoType[eType].insert(make_pair(strPrototypeKey, pObj));
	}
	static Object* FindPrototype(const string& strPrototypeKey);
	static Object* FindPrototype(OBJECT_TYPE eType, const string& strPrototypeKey);
	static Object* CreateCloneObject(const string& strPrototypeKey, const string& strTag, Layer* pLayer);
	bool Init();
};


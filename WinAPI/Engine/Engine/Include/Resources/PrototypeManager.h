#pragma once
#include "../framework.h"

class Object;
class Layer;
class PrototypeManager
{
	friend class App;
	friend class UITileSelect;
	DECLARE_SINGLE(PrototypeManager);
private:
	static unordered_map<string, Object*> m_mapProtoType[OBJ_END];
	static Object* FindPrototype(const string& strPrototypeKey);
	static Object* FindPrototype(OBJECT_TYPE eType, const string& strPrototypeKey);
	unordered_map<string, Object*>* GetPrototypes(OBJECT_TYPE eType) const { return &m_mapProtoType[eType]; }
public:
	void LoadTileObjectInFolder(OBJECT_TYPE eType, const wchar_t* pBaseFolderName, const string& strPathKey = TEXTURE_PATH);
	static bool RegisterProtoType(OBJECT_TYPE eType, const string& strPrototypeKey, Object* pObj)
	{
		if (FindPrototype(eType, strPrototypeKey))
			return false;

		m_mapProtoType[eType].insert(make_pair(strPrototypeKey, pObj));
		return true;
	}
	static Object* CreateCloneObject(const string& strPrototypeKey, const string& strTag, Layer* pLayer);
private:
	bool Init();
};


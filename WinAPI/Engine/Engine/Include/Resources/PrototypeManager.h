#pragma once
#include "../framework.h"

class Object;
class Layer;
class PrototypeManager
{
	DECLARE_SINGLE(PrototypeManager);
private:
	static unordered_map<string, Object*> m_mapProtoType;
	static Object* FindPrototype(const string& strPrototypeKey);
public:
	static bool RegisterProtoType(const string& strPrototypeKey, Object* pObj)
	{
		if (FindPrototype(strPrototypeKey))
			return false;

		m_mapProtoType.insert(make_pair(strPrototypeKey, pObj));
	}
	static Object* CreateCloneObject(const string& strPrototypeKey, const string& strTag, Layer* pLayer);
public:
	bool Init();
};


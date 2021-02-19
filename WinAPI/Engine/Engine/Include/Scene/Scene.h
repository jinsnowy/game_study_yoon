#pragma once
#include "../framework.h"

class Layer;

class Scene
{
	friend class Object;
	friend class SceneManager;
private:
	static unordered_map<string, class Object*> m_mapProtoType;
	static Object* FindPrototype(const string& strkey);
public:
	static void ErasePrototype(const string& strPrototypeKey);
	static void EraseAllPrototypes();
	template<typename T>
	static T* CreatePrototype(const string& strTag)
	{
		T* pObj = new T;
		pObj->SetTag(strTag);

		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return nullptr;
		}

		pObj->AddRef();
		m_mapProtoType.insert(make_pair(strTag, pObj));

		return pObj;
	}
protected:
	Scene();
	virtual ~Scene() = 0;
	list<Layer*> m_LayerList;
public:
	Layer* FindLayer(const string& tag);
	Layer* CreateLayer(const string& tag, int zOrder = 0);
	static bool LayerSort(const Layer* pL1, const Layer* pL2);

	virtual bool Init();
	virtual void Input(float dt);
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
};


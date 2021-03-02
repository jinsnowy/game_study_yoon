#pragma once
#include "../framework.h"

class Layer;

class Scene
{
	friend class Object;
	friend class SceneManager;
private:
	static unordered_map<string, class Object*> m_mapProtoType[SC_END];
	static Object* FindPrototype(const string& strkey, SCENE_CREATE sc);
	static void ChangeProtoType();
public:
	static void ErasePrototype(const string& strPrototypeKey, SCENE_CREATE sc);
	static void EraseAllPrototypes(SCENE_CREATE sc);
	template<typename T>
	static T* CreatePrototype(const string& strTag, SCENE_CREATE sc)
	{
		T* pObj = new T;
		pObj->SetTag(strTag);

		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return nullptr;
		}

		pObj->AddRef();
		m_mapProtoType[sc].insert(make_pair(strTag, pObj));

		return pObj;
	}
protected:
	Scene();
	virtual ~Scene() = 0;
	list<Layer*> m_LayerList;
	SCENE_CREATE m_eSceneType;

public:
	void DrawBackGround(HDC hdc, COLORREF color);
public:
	void SetSceneType(SCENE_CREATE eType){ m_eSceneType = eType; }
	SCENE_CREATE GetSceneType() const { return m_eSceneType; }
	Layer* FindLayer(const string& tag);
	Layer* CreateLayer(const string& tag, int zOrder = 0);
	static bool LayerSort(const Layer* pL1, const Layer* pL2);
	void ChangeShowMode();
	virtual bool Init();
	virtual void Input(float dt);
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
};


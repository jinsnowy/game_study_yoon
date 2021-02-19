#pragma once
#include "../framework.h"
#include "../Scene/Layer.h"
#include "Ref.h"

class Object : public Ref
{
private:
	static list<Object*> m_ObjList;
	static unordered_map<string, Object*> m_mapProtoType;

public:
	static void AddObject(Object* pObj);
	static Object* FindObject(const string& tag);
	static void EraseObject(Object* pObj);
	static void EraseObject(const string& tag);
	static void ErasePrototype(const string& strPrototypeKey);
	static void EraseAllObjects();
	static void EraseAllPrototypes();
protected:
	// ¾À°ú ·¹ÀÌ¾î
	class Scene* m_pScene;
	class Layer* m_pLayer;
	
	Pos m_Pos;
	Pos m_Pivot;
	Size  m_Size;
	string m_Tag;
public:
	Object();
	virtual ~Object();

	// ¾À°ú ·¹ÀÌ¾î Get/Set
	void SetScene(class Scene* pScene) { m_pScene = pScene; }
	void SetLayer(class Layer* pLayer) { m_pLayer = pLayer; }
	class Scene* GetScene() const { return m_pScene; }
	class Layer* GetLayer() const { return m_pLayer; }

	// °´Ã¼ º¯¼ö Get/Set

	Pos GetPos() const { return m_Pos; }
	Size GetSize() const { return m_Size; }
	string GetTag() const { return m_Tag; }

	void SetPos(float x, float y) { m_Pos.x = x; m_Pos.y = y; }
	void SetSize(float x, float y) { m_Size.x = x; m_Size.y = y; }
	void SetPos(const Pos& pos) { m_Pos = pos; }
	void SetSize(const Size& size) { m_Size = size; }
	void SetTag(const string& tag) { m_Tag = tag; }
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual Object* Clone() = 0;
public:
	template<typename T>
	static T* CreateObject(const string& strTag, class Layer* pLayer = nullptr)
	{
		T* pObj = new T;
		pObj->SetTag(strTag);

		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return nullptr;
		}

		if (pLayer)
		{
			pLayer->AddObject(pObj);
		}

		AddObject(pObj);

		pObj->AddRef();
		return pObj;
	}
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
		
		m_mapProtoType.insert(make_pair<string, T*>(strTag, pObj));

		pObj->AddRef();
		return pObj;
	}
	static Object* CreateCloneObject(const string& strPrototypeKey, const string& strTag, class Layer* pLayer = nullptr);
private:
	static Object* FindPrototype(const string& strkey);
};


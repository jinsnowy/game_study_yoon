#pragma once
#include "../framework.h"
#include "../Scene/Layer.h"

class Object
{
private:
	static std::list<std::shared_ptr<Object>> m_ObjList;
	static std::unordered_map<std::string, Object*> m_mapProtoType;

public:
	static void AddObject(Object* pObj);
	static Object* FindObject(const std::string& tag);
	static void EraseObject(Object* pObj);
	static void EraseObject(const std::string& tag);
	static void ErasePrototype(const std::string& strPrototypeKey);
	static void EraseAllObjects();
	static void EraseAllPrototypes();
protected:
	// ���� ���̾�
	class Scene* m_pScene;
	class Layer* m_pLayer;

	// ��ü ����
	bool m_bEnable;
	bool m_bLife;
	
	Pos m_Pos;
	Pos m_Pivot;
	Size  m_Size;
	std::string m_Tag;
public:
	Object();
	virtual ~Object();

	// ���� ���̾� Get/Set
	void SetScene(class Scene* pScene) { m_pScene = pScene; }
	void SetLayer(class Layer* pLayer) { m_pLayer = pLayer; }
	class Scene* GetScene() const { return m_pScene; }
	class Layer* GetLayer() const { return m_pLayer; }

	// ��ü ���� Get/Set
	bool GetEnable() const { return m_bEnable; }
	bool GetLife() const { return m_bLife; }
	Pos GetPos() const { return m_Pos; }
	Size GetSize() const { return m_Size; }
	std::string GetTag() const { return m_Tag; }

	void Die() { m_bLife = false; }
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void SetPos(float x, float y) { m_Pos.x = x; m_Pos.y = y; }
	void SetSize(float x, float y) { m_Size.x = x; m_Size.y = y; }
	void SetPos(const Pos& pos) { m_Pos = pos; }
	void SetSize(const Size& size) { m_Size = size; }
	void SetTag(const std::string& tag) { m_Tag = tag; }
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
	static T* CreateObject(const std::string& strTag, class Layer* pLayer = nullptr)
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

		return pObj;
	}
	template<typename T>
	static T* CreatePrototype(const std::string& strTag)
	{
		T* pObj = new T;
		pObj->SetTag(strTag);

		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return nullptr;
		}
		
		m_mapProtoType.insert(std::make_pair<std::string, T*>(strTag, pObj));

		return pObj;
	}
	static Object* CreateCloneObject(const std::string& strPrototypeKey, const std::string& strTag, class Layer* pLayer = nullptr);
private:
	static Object* FindPrototype(const std::string& strkey);
};


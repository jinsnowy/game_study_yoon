#pragma once
#include "../framework.h"
#include "../Scene/Layer.h"
#include "../Collider/Collider.h"

class Object : public Ref
{
private:
	static list<Object*> m_ObjList;
public:
	static void AddObject(Object* pObj);
	static Object* FindObject(const string& tag);
	static void EraseObject(Object* pObj);
	static void EraseObject(const string& tag);
	static void EraseAllObjects();
protected:
	// 씬, 레이어, 텍스쳐, 콜라이더
	class Scene* m_pScene;
	class Layer* m_pLayer;
	class Texture* m_pTexture;
	list<Collider*> m_ColliderList;
	class Animation* m_pAnimation;
public:
	class Animation* CreateAnimation(const string& strTag);
	const list<Collider*>* GetColliderList() const 
	{
		return &m_ColliderList;
	}
	template<typename T>
	void AddCollisionFunction(const string& strTag, COLLISION_STATE eState, T* pObj, void (T::* pFunc)(Collider*, Collider*, float))
	{
		list<Collider*>::iterator iter;
		list<Collider*>::iterator iterEnd = m_ColliderList.end();

		for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTag() == strTag)
			{
				(*iter)->AddCollisionFunction(eState, pObj, pFunc);
				break;
			}
		}
	}
public:
	template<typename T>
	T* AddCollider(const string& strTag)
	{
		T* pCollider = new T;

		pCollider->SetObj(this);
		pCollider->SetTag(strTag);

		if (!pCollider->Init())
		{
			SAFE_RELEASE(pCollider);
			return nullptr;
		}

		pCollider->AddRef();
		m_ColliderList.push_back(pCollider);

		return pCollider;
	}
	bool CheckCollider() const
	{
		return !m_ColliderList.empty();
	}
protected:
	bool m_blsPhysics;
	float m_fGravityTime;
public:
	void ClearGravity()
	{
		m_fGravityTime = 0.f;
	}
	void SetPhysics(bool bPhysics)
	{
		m_blsPhysics = bPhysics;
	}
	bool GetPhysics() const
	{
		return m_blsPhysics;
	}
protected:
	Pos m_Pos;
	Pos m_Pivot;
	Size  m_Size;
public:
	Object();
	Object(const Object& obj);
	virtual ~Object();

	// 씬과 레이어 Get/Set
	void SetScene(class Scene* pScene) { m_pScene = pScene; }
	void SetLayer(class Layer* pLayer) { m_pLayer = pLayer; }
	class Scene* GetScene() const { return m_pScene; }
	class Layer* GetLayer() const { return m_pLayer; }

	// 객체 변수 Get/Set
	float GetLeft() const { return m_Pos.x - m_Size.x * m_Pivot.x; }
	float GetTop() const { return m_Pos.y - m_Size.y * m_Pivot.y; }
	float GetRight() const { return GetLeft() + m_Size.x; }
	float GetBottom() const { return GetTop() + m_Size.y; }
	Pos GetCenter() { return Pos((GetLeft()+GetRight())/2.f, (GetTop()+GetBottom())/2.f); }
	Pos GetPos() const { return m_Pos; }
	Pos GetPivot() const { return m_Pivot; }
	Size GetSize() const { return m_Size; }

	void SetPos(float x, float y) { m_Pos.x = x; m_Pos.y = y; }
	void SetSize(float x, float y) { m_Size.x = x; m_Size.y = y; }
	void SetPivot(float x, float y) { m_Pivot.x = x; m_Pivot.y = y; }
	void SetPos(const Pos& pos) { m_Pos = pos; }
	void SetSize(const Size& size) { m_Size = size; }
	void SetPivot(const Pos& pivot) { m_Pivot = pivot; }
public:
	void SetTexture(class Texture* pTexture);
	void SetTexture(const string& strKey, const wchar_t* pFileName = nullptr, const string& strPathKey = TEXTURE_PATH);
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
		return pObj;
	}
	Object* CreateCloneObject(const string& strPrototypeKey, const string& strTag, class Layer* pLayer);
};


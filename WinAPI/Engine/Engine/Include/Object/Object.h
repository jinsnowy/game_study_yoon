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
	// ��, ���̾�, �ؽ���, �ݶ��̴�
	bool m_bEnableAnimation;
	class Scene* m_pScene;
	class Layer* m_pLayer;
	class Texture* m_pTexture;
	list<Collider*> m_ColliderList;
	class Animation* m_pAnimation;
public:
	void SetEnableAnimation(bool enabled) { m_bEnableAnimation = enabled; }
	void SetClipColorKey(const string& strName, unsigned char r, unsigned char g, unsigned char b);
	class Animation* CreateAnimation(const string& strTag);
	bool AddAnimationClip(const string& strName,
							ANIMATION_TYPE eType, ANIMATION_OPTION eOption,
							float	fAnimationTime,
							float   fAnimationLimitTime,
							int		iFrameMaxX,
							int		iFrameMaxY,
							int		iStartX,
							int		iStartY,
							int		iLengthX,
							int		iLengthY,
							float	fOptionLimitTime,
							const string& strTexKey,
							const wchar_t* pFileName,
							const string& strPathKey = TEXTURE_PATH);
	bool AddAnimationClip(const string& strName,
					ANIMATION_TYPE eType, ANIMATION_OPTION eOption,
					float	fAnimationTime,
					float   fAnimationLimitTime,
					int		iFrameMaxX,
					int		iFrameMaxY,
					int		iStartX,
					int		iStartY,
					int		iLengthX,
					int		iLengthY,
					float	fOptionLimitTime,
					const string& strTexKey,
					const vector<wstring>& vecFileName,
					const string& strPathKey = TEXTURE_PATH);
public:
	const list<Collider*>* GetColliderList() const
	{
		return &m_ColliderList;
	}
	Collider* GetCollider(const string& strTag);

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
	Pos   m_tPos;
	Pos   m_tPivot;
	Size  m_tImageOffset;
	Size  m_tSize;
public:
	Object();
	Object(const Object& obj);
	virtual ~Object();

	// ���� ���̾� Get/Set
	void SetScene(class Scene* pScene) { m_pScene = pScene; }
	void SetLayer(class Layer* pLayer) { m_pLayer = pLayer; }
	class Scene* GetScene() const { return m_pScene; }
	class Layer* GetLayer() const { return m_pLayer; }

	// ��ü ���� Get/Set
	float GetLeft() const { return m_tPos.x - m_tSize.x * m_tPivot.x; }
	float GetTop() const { return m_tPos.y - m_tSize.y * m_tPivot.y; }
	float GetRight() const { return GetLeft() + m_tSize.x; }
	float GetBottom() const { return GetTop() + m_tSize.y; }
	Pos GetCenter() { return Pos((GetLeft()+GetRight())/2.f, (GetTop()+GetBottom())/2.f); }
	Pos GetPos() const { return m_tPos; }
	Pos GetPivot() const { return m_tPivot; }
	Size GetSize() const { return m_tSize; }
	Size GetImageSize() const;
	Size GetImageOffset() const { return m_tImageOffset; }

	void SetAsTextureSize();
	void SetPos(float x, float y) { m_tPos.x = x; m_tPos.y = y; }
	void SetSize(float x, float y) { m_tSize.x = x; m_tSize.y = y; }
	void SetPivot(float x, float y) { m_tPivot.x = x; m_tPivot.y = y; }
	void SetPos(const Pos& pos) { m_tPos = pos; }
	void SetSize(const Size& size) { m_tSize = size; }
	void SetPivot(const Pos& pivot) { m_tPivot = pivot; }
	void SetImageOffset(const Size& tOffset) { m_tImageOffset = tOffset; }
	void SetImageOffset(float x, float y) { m_tImageOffset = Size(x, y); }
public:
	void SetTexture(class Texture* pTexture);
	void SetTexture(const string& strKey, const wchar_t* pFileName = nullptr, const string& strPathKey = TEXTURE_PATH);
	void SetColorKey(unsigned char r, unsigned char g, unsigned char b);
	void SetAnimationClipColorKey(const string& strClip, unsigned char r, unsigned char g, unsigned char b);
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual Object* Clone() = 0;
public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
public:
	void SaveFromFile(FILE* pFile);
	void LoadFromFile(FILE* pFile);
	void SaveFromPath(const char* pFileName, const string& strPathKey = DATA_PATH);
	void SaveFromFullPath(const char* pFullPath);
	void LoadFromPath(const char* pFileNmae, const string& strPathKey = DATA_PATH);
	void LoadFromFullPath(const char* pFullPath);
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
	Object* CreateCloneObject(const string& strPrototypeKey, const string& strTag, SCENE_CREATE sc, class Layer* pLayer);
};


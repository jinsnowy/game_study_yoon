#pragma once
#include "../framework.h"

class Object
{
protected:
	// ¾À°ú ·¹ÀÌ¾î
	class Scene* m_pScene;
	class Layer* m_pLayer;

	// °´Ã¼ º¯¼ö
	Pos m_Pos;
	Pos m_Pivot;
	Size  m_Size;
	std::string m_Tag;

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
	std::string GetTag() const { return m_Tag; }
	void SetPos(const Pos& pos) { m_Pos = pos; }
	void SetSize(const Size& size) { m_Size = size; }
	void SetTag(const std::string& tag) { m_Tag = tag; }
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
public:
	template<typename T>
	static T* CreateObject(const std::string& strTag, class Layer* pLayer = nullptr)
	{
		T* pObj = new T;

		if (!pObj->Init())
		{
			SAFE_DELETE(pObj);
			return nullptr;
		}

		if (pLayer)
		{
			pLayer->AddObject(pObj);
		}
		return pObj;
	}
};


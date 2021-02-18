#pragma once
#include "../framework.h"

class Layer
{
private:
	friend class Scene;
	Layer();
private:
	bool		m_bEnable;
	bool		m_bLife;
	int			m_zOrder;
	std::string	m_tag;
	class Scene* m_pScene;
	std::list<std::shared_ptr<class Object>> m_ObjList;
public:
	~Layer();

	void AddObject(class Object* pObj);
	// Get/Set method
	bool GetEnable() const { return m_bEnable; }
	bool GetLife() const { return m_bLife; }

	void Die() { m_bLife = false; }
	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	int  GetZOrder() const { return m_zOrder; }
	std::string GetTag() const { return m_tag; }
	class Scene* GetScene() const { return m_pScene; }
	void SetZOrder(int zOrder) { m_zOrder = zOrder; }
	void SetTag(const std::string& tag) { m_tag = tag; }
	void SetScene(class Scene* pScene) { m_pScene = pScene; }

public:
	void Input(float dt);
	void Update(float dt);
	void LateUpdate(float dt);
	void Collision(float dt);
	void Draw(HDC hdc, float dt);
};


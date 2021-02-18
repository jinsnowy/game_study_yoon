#pragma once
#include "../framework.h"
#include "../Application/Graphics.h"

class Scene;
class Object;

class Layer
{
private:
	friend class Scene;
	Layer();
private:
	int				m_zOrder;
	std::string		m_tag;
	Scene* m_pScene;
private:
	std::vector<std::shared_ptr<Object>> m_ObjList;
public:
	~Layer();
	void SetZOrder(int zOrder) { m_zOrder = zOrder; }
	void SetTag(const std::string& tag) { m_tag = tag; }
	void SetScene(Scene* pScene) { m_pScene = pScene; }
	int  GetZOrder() const { return m_zOrder;  }
	std::string GetTag() const { return m_tag; }
	Scene* GetScene() const { return m_pScene; }
public:
	void Input(float dt);
	void Update(float dt);
	void LateUpdate(float dt);
	void Collision(float dt);
	void Draw(Graphics& gfx, float dt);
};


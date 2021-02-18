#pragma once
#include "../framework.h"

class Layer;

class Scene
{
protected:
	friend class SceneManager;
	Scene();
	virtual ~Scene() = 0;
	std::list<std::shared_ptr<Layer>> m_LayerList;
public:
	Layer* CreateLayer(const std::string& strTag, int iZOrder = 0);
	static bool LayerSort(const std::shared_ptr<Layer> pL1, const std::shared_ptr<Layer> pL2);

	virtual bool Init();
	virtual void Input(float dt);
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
};


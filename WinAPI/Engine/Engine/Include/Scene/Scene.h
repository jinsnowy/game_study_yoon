#pragma once
#include "../framework.h"
#include "../Application/Graphics.h"

class Layer;

class Scene
{
protected:
	friend class SceneManager;
	Scene();
	virtual ~Scene() = 0;
	std::vector<std::shared_ptr<Layer>> m_LayerList;
public:
	Layer* CreateLayer(const std::string& strTag, int iZOrder = 0);
	static bool LayerSort(const Layer* const pL1, const Layer* const pL2);

	virtual bool Init();
	virtual void Input(float dt);
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(Graphics& gfx, float dt);
};


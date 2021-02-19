#pragma once
#include "../framework.h"

class Layer;

class Scene
{
friend class SceneManager;
protected:
	Scene();
	virtual ~Scene() = 0;
	list<shared_ptr<Layer>> m_LayerList;
public:
	Layer* FindLayer(const string& tag);
	Layer* CreateLayer(const string& tag, int zOrder = 0);
	static bool LayerSort(const shared_ptr<Layer> pL1, const shared_ptr<Layer> pL2);

	virtual bool Init();
	virtual void Input(float dt);
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
};


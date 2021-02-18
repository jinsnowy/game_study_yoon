#include "Scene.h"
#include "Layer.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	Delete_SharedPtr_VecList(m_LayerList);
}

Layer* Scene::FindLayer(const std::string& tag)
{
	for (auto it = m_LayerList.begin(); it != m_LayerList.end(); it++)
	{
		if ((*it)->GetTag() == tag)
			return it->get();
	}
	return nullptr;
}

Layer* Scene::CreateLayer(const std::string& tag, int zOrder)
{
	Layer* pLayer = new Layer;

	pLayer->SetTag(tag);
	pLayer->SetZOrder(zOrder);
	pLayer->SetScene(this);

	m_LayerList.emplace_back(pLayer);
	
	if (m_LayerList.size() >= 2)
	{
		m_LayerList.sort(LayerSort);
	}
	return pLayer;
}

bool Scene::LayerSort(const std::shared_ptr<Layer> pL1, const std::shared_ptr<Layer> pL2)
{
	return pL1->GetZOrder() < pL2->GetZOrder();
}

bool Scene::Init()
{
	return true;
}

void Scene::Input(float dt)
{
	for (auto it = m_LayerList.begin(); it != m_LayerList.end(); it++)
	{
		(*it)->Input(dt);
	}
}

void Scene::Update(float dt)
{
	for (auto it = m_LayerList.begin(); it != m_LayerList.end(); it++)
	{
		(*it)->Update(dt);
	}
}

void Scene::LateUpdate(float dt)
{
	for (auto it = m_LayerList.begin(); it != m_LayerList.end(); it++)
	{
		(*it)->LateUpdate(dt);
	}
}

void Scene::Collision(float dt)
{
	for (auto it = m_LayerList.begin(); it != m_LayerList.end(); it++)
	{
		(*it)->Collision(dt);
	}
}

void Scene::Draw(HDC hdc, float dt)
{
	for (auto it = m_LayerList.begin(); it != m_LayerList.end(); it++)
	{
		(*it)->Draw(hdc, dt);
	}
}

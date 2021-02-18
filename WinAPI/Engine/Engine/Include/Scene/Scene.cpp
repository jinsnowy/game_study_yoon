#include "Scene.h"
#include "Layer.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	Safe_Delete_VecList(m_LayerList);
}

Layer* Scene::CreateLayer(const std::string& strTag, int iZOrder)
{
	std::shared_ptr<Layer> pLayer = std::make_shared<Layer>();

	pLayer->SetTag(strTag);
	pLayer->SetZOrder(iZOrder);
	pLayer->SetScene(this);
	m_LayerList.push_back(pLayer);
	
	if (m_LayerList.size() >= 2)
	{
		std::sort(m_LayerList.begin(), m_LayerList.end(), LayerSort);
	}
	return pLayer.get();
}

bool Scene::LayerSort(const Layer* const pL1, const Layer* const pL2)
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

void Scene::Draw(Graphics& gfx, float dt)
{
	for (auto it = m_LayerList.begin(); it != m_LayerList.end(); it++)
	{
		(*it)->Draw(gfx, dt);
	}
}

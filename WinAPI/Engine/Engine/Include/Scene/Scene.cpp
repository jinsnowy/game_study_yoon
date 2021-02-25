#include "Scene.h"
#include "Layer.h"
#include "../Object/Object.h"

unordered_map<string, Object*> Scene::m_mapProtoType;

Scene::Scene()
{
	Layer* pLayer = CreateLayer("UI", INT_MAX);
	pLayer = CreateLayer("HUD", INT_MAX - 1);
	pLayer = CreateLayer("Default", 1);
	pLayer = CreateLayer("Stage");
}

Scene::~Scene()
{
	EraseAllPrototypes();
	Safe_Delete_VecList(m_LayerList);
}

void Scene::ErasePrototype(const string& strPrototypeKey)
{
	auto it = m_mapProtoType.find(strPrototypeKey);
	if (it == m_mapProtoType.end())
		return;

	SAFE_RELEASE(it->second);
	m_mapProtoType.erase(it);
}

void Scene::EraseAllPrototypes()
{
	Safe_Release_Map(m_mapProtoType);
}

Layer* Scene::FindLayer(const string& tag)
{
	for (auto it = m_LayerList.begin(); it != m_LayerList.end(); it++)
	{
		if ((*it)->GetTag() == tag)
			return *it;
	}
	return nullptr;
}

Layer* Scene::CreateLayer(const string& tag, int zOrder)
{
	Layer* pLayer = new Layer;

	pLayer->SetTag(tag);
	pLayer->SetZOrder(zOrder);
	pLayer->SetScene(this);

	m_LayerList.push_back(pLayer);
	
	if (m_LayerList.size() >= 2)
	{
		m_LayerList.sort(LayerSort);
	}
	return pLayer;
}

bool Scene::LayerSort(const Layer* pL1, const Layer* pL2)
{
	return pL1->GetZOrder() < pL2->GetZOrder();
}

bool Scene::Init()
{
	return true;
}

void Scene::Input(float dt)
{
	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
		if (!(*it)->GetEnable())
		{
			++it;
			continue;
		}

		(*it)->Input(dt);

		if (!(*it)->GetLife())
		{
			SAFE_DELETE(*it);
			it = m_LayerList.erase(it);
			iterEnd = m_LayerList.end();
		}
		else ++it;
	}
}

void Scene::Update(float dt)
{
	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
		if (!(*it)->GetEnable())
		{
			++it;
			continue;
		}

		(*it)->Update(dt);

		if (!(*it)->GetLife())
		{
			SAFE_DELETE(*it);
			it = m_LayerList.erase(it);
			iterEnd = m_LayerList.end();
		}
		else ++it;
	}
}

void Scene::LateUpdate(float dt)
{
	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
		if (!(*it)->GetEnable())
		{
			++it;
			continue;
		}

		(*it)->LateUpdate(dt);

		if (!(*it)->GetLife())
		{
			SAFE_DELETE(*it);
			it = m_LayerList.erase(it);
			iterEnd = m_LayerList.end();
		}
		else ++it;
	}
}

void Scene::Collision(float dt)
{
	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
		if (!(*it)->GetEnable())
		{
			++it;
			continue;
		}

		(*it)->Collision(dt);

		if (!(*it)->GetLife())
		{
			SAFE_DELETE(*it);
			it = m_LayerList.erase(it);
			iterEnd = m_LayerList.end();
		}
		else ++it;
	}
}

void Scene::Draw(HDC hdc, float dt)
{
	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
		if (!(*it)->GetEnable())
		{
			++it;
			continue;
		}

		(*it)->Draw(hdc, dt);

		if (!(*it)->GetLife())
		{
			SAFE_DELETE(*it);
			it = m_LayerList.erase(it);
			iterEnd = m_LayerList.end();
		}
		else ++it;
	}
}

Object* Scene::FindPrototype(const string& strkey)
{
	auto it = m_mapProtoType.find(strkey);
	if (it == m_mapProtoType.end())
		return nullptr;
	return it->second;
}

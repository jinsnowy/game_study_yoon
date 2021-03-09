#include "Scene.h"
#include "Layer.h"
#include "../Object/Object.h"
#include "../Scene/SceneManager.h"
#include "../Application/Window.h"
#include "../Core/Camera.h"

unordered_map<string, Object*> Scene::m_mapProtoType[SC_END];

Scene::Scene()
{
	Layer* pLayer = CreateLayer("UI", INT_MAX);
	pLayer = CreateLayer("HUD", INT_MAX - 1);
	pLayer = CreateLayer("Static", 6);
	pLayer = CreateLayer("Effect", 3);
	pLayer = CreateLayer("Object", 2);
	pLayer = CreateLayer("Ground", 0);
	m_eSceneType = SC_CURRENT;
}

Scene::~Scene()
{
	EraseAllPrototypes(m_eSceneType);
	Safe_Delete_VecList(m_LayerList);
}

void Scene::ErasePrototype(const string& strPrototypeKey, SCENE_CREATE sc)
{
	auto it = m_mapProtoType[sc].find(strPrototypeKey);
	if (it == m_mapProtoType[sc].end())
		return;

	SAFE_RELEASE(it->second);
}

void Scene::EraseAllPrototypes(SCENE_CREATE sc)
{
	Safe_Release_Map(m_mapProtoType[sc]);
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
	DrawHDCWithColor(hdc, GETRESOLUTION.x, GETRESOLUTION.y, RGB(0, 0, 0));

	STAGE_SHOW eShowMode = SCENE_MANAGER->GetShowMode();

	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
#ifdef _DEBUG
		const string& layerTag = (*it)->GetTag();
		if (layerTag == "Ground")
		{
			if (eShowMode != SHOW_ALL && eShowMode != SHOW_GROUND)
			{
				++it;
				continue;
			}
		}
		else if (layerTag == "Static")
		{
			if (eShowMode != SHOW_ALL && eShowMode != SHOW_STATIC)
			{
				++it;
				continue;
			}
		}
		else if (layerTag == "Object")
		{
			if (eShowMode != SHOW_ALL && eShowMode != SHOW_OBJECT)
			{
				++it;
				continue;
			}
		}
#endif

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

Object* Scene::FindPrototype(const string& strkey, SCENE_CREATE sc)
{
	auto it = m_mapProtoType[sc].find(strkey);
	if (it == m_mapProtoType[sc].end())
		return nullptr;
	return it->second;
}

void Scene::ChangeProtoType()
{
	EraseAllPrototypes(SC_CURRENT);
	m_mapProtoType[SC_CURRENT] = std::move(m_mapProtoType[SC_NEXT]);
	m_mapProtoType[SC_NEXT].clear();
}

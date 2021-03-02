#include "Scene.h"
#include "Layer.h"
#include "../Object/Object.h"
#include "../Core/Camera.h"

unordered_map<string, Object*> Scene::m_mapProtoType[SC_END];

Scene::Scene()
{
	Layer* pLayer = CreateLayer("UI", INT_MAX);
	pLayer = CreateLayer("HUD", INT_MAX - 1);
	pLayer = CreateLayer("OnAir", 2);
	pLayer = CreateLayer("Object", 1);
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
	m_mapProtoType[sc].erase(it);
}

void Scene::EraseAllPrototypes(SCENE_CREATE sc)
{
	Safe_Release_Map(m_mapProtoType[sc]);
}

void Scene::DrawBackGround(HDC hdc, COLORREF color)
{
	RESOLUTION tPos = CAMERA->GetWorldRS();

	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, CreateSolidBrush(color));

	Rectangle(hdc, 0, 0, tPos.x, tPos.y);

	DeleteObject(SelectObject(hdc, OldBrush));
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

void Scene::ChangeShowMode()
{
	if (GetAsyncKeyState(VK_F1) & 0x8000)
	{
		m_eCurShowMode = SHOW_ALL;
	}
	if (GetAsyncKeyState(VK_F2) & 0x8000)
	{
		m_eCurShowMode = SHOW_GROUND;
	}
	if (GetAsyncKeyState(VK_F3) & 0x8000)
	{
		m_eCurShowMode = SHOW_OBJECT;
	}
	if (GetAsyncKeyState(VK_F4) & 0x8000)
	{
		m_eCurShowMode = SHOW_ONAIR;
	}
}

bool Scene::Init()
{
	return true;
}

void Scene::Input(float dt)
{
	ChangeShowMode();

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
	DrawBackGround(hdc, RGB(0, 0, 0));

	auto iterEnd = m_LayerList.end();
	for (auto it = m_LayerList.begin(); it != iterEnd;)
	{
		const string& layerTag = (*it)->GetTag();
		if (layerTag == "Ground")
		{
			if (m_eCurShowMode != SHOW_ALL && m_eCurShowMode != SHOW_GROUND)
			{
				++it;
				continue;
			}
		}
		else if (layerTag == "Object")
		{
			if (m_eCurShowMode != SHOW_ALL && m_eCurShowMode != SHOW_OBJECT)
			{
				++it;
				continue;
			}
		}
		else if (layerTag == "OnAir")
		{
			if (m_eCurShowMode != SHOW_ALL && m_eCurShowMode != SHOW_ONAIR)
			{
				++it;
				continue;
			}
		}

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

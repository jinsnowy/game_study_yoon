#include "SceneManager.h"
#include "InGameScene.h"
#include "StartScene.h"
#include "../Collider/CollisionManager.h"

DEFINITION_SINGLE(SceneManager)

SceneManager::SceneManager()
	:
	m_pScene(nullptr),
	m_pNextScene(nullptr)
{
}

SceneManager::~SceneManager()
{
	SAFE_DELETE(m_pScene);
}

bool SceneManager::Init()
{
	m_pScene = CreateScene<StartScene>(SCENE_CREATE::SC_CURRENT);
	return true;
}

void SceneManager::Input(float dt)
{
	m_pScene->Input(dt);
}

SCENE_CHANGE SceneManager::Update(float dt)
{
	m_pScene->Update(dt);
	return ChangeScene();
}

SCENE_CHANGE SceneManager::LateUpdate(float dt)
{
	m_pScene->LateUpdate(dt);
	return ChangeScene();
}

void SceneManager::Collision(float dt)
{
	m_pScene->Collision(dt);
}

void SceneManager::Draw(HDC hdc, float dt)
{
	m_pScene->Draw(hdc, dt);
}

SCENE_CHANGE SceneManager::ChangeScene()
{
	if (m_pNextScene)
	{
		SAFE_DELETE(m_pScene);
		m_pScene = m_pNextScene;
		m_pNextScene = nullptr;

		COLLISION_MANAGER->Clear();

		m_pScene->SetSceneType(SC_CURRENT);
		Scene::ChangeProtoType();

		return SC_CHANGE;
	}
	return SC_NONE;
}

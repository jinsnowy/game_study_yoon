#include "SceneManager.h"
#include "InGameScene.h"

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
	m_pScene = CreateScene<InGameScene>(SCENE_CREATE::SC_CURRENT);
	return true;
}

void SceneManager::Input(float dt)
{
	m_pScene->Input(dt);
}

void SceneManager::Update(float dt)
{
	m_pScene->Update(dt);
}

void SceneManager::LateUpdate(float dt)
{
	m_pScene->LateUpdate(dt);
}

void SceneManager::Collision(float dt)
{
	m_pScene->Collision(dt);
}

void SceneManager::Draw(HDC hdc, float dt)
{
	m_pScene->Draw(hdc, dt);
}

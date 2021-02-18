#include "SceneManager.h"

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

	return true;
}

void SceneManager::Input(float dt)
{
}

void SceneManager::Update(float dt)
{
}

void SceneManager::LateUpdate(float dt)
{
}

void SceneManager::Collision(float dt)
{
}

void SceneManager::Draw(Graphics& gfx, float dt)
{
}

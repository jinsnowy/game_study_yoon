#pragma once
#include "../framework.h"

class Scene;
class SceneManager
{
	DECLARE_SINGLE(SceneManager)
private:
private:
	Scene* m_pScene;
	Scene* m_pNextScene;
public:
	class Scene* GetScene() const { return m_pScene; }
public:
	bool Init();
	void Input(float dt);
	void Update(float dt);
	void LateUpdate(float dt);
	void Collision(float dt);
	void Draw(HDC hdc, float dt);

	template<typename T>
	T* CreateScene(SCENE_CREATE sc)
	{
		T* pScene = new T;
		if (!pScene->Init())
		{
			SAFE_DELETE(pScene);
			return nullptr;
		}
		switch (sc)
		{
		case SC_CURRENT:
			SAFE_DELETE(m_pScene);
			m_pScene = pScene;
			break;
		case SC_NEXT:
			SAFE_DELETE(m_pNextScene);
			m_pNextScene = pScene;
			break;
		}
		return pScene;
	}
};


#pragma once
#include "../framework.h"

class Object;
class Scene;
class SceneManager
{
	DECLARE_SINGLE(SceneManager)
private:

	STAGE_SHOW m_eCurShowMode = SHOW_ALL;
	Scene* m_pScene;
	Scene* m_pNextScene;
	SCENE_CHANGE ChangeScene();
public:
	STAGE_SHOW GetShowMode() const { return m_eCurShowMode; };
	void SetShowmode(STAGE_SHOW eState) { m_eCurShowMode = eState; }
	void ChangeShowMode();
	class Scene* GetScene() const { return m_pScene; }
public:
	bool Init();
	void Input(float dt);
	SCENE_CHANGE Update(float dt);
	SCENE_CHANGE LateUpdate(float dt);
	void Collision(float dt);
	void Draw(HDC hdc, float dt);
	template<typename T>
	T* CreateScene(SCENE_CREATE sc = SC_CURRENT)
	{
		T* pScene = new T;

		pScene->SetSceneType(sc);
		m_eCurShowMode = SHOW_ALL;

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


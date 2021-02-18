#pragma once
#include "../framework.h"
#include "../Application/Graphics.h"

class Scene;
class SceneManager
{
	DECLARE_SINGLE(SceneManager)
private:
	enum class SCENE_CREATE
	{
		SC_CURRENT,
		SC_NEXT
	};
private:
	Scene* m_pScene;
	Scene* m_pNextScene;

public:
	bool Init();
	void Input(float dt);
	void Update(float dt);
	void LateUpdate(float dt);
	void Collision(float dt);
	void Draw(Graphics& gfx, float dt);

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
		case SCENE_CREATE::SC_CURRENT:
				SAFE_DELETE(m_pScene);
				m_pScene = pScene;
				break;
		case SCENE_CREATE::SC_NEXT
			SAFE_DELETE(m_pNextScene);
			m_pNextScene = pScene;
			break;
		}
	}
};


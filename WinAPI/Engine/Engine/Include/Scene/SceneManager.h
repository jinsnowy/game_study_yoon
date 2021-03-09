#pragma once
#include "../framework.h"

class Object;
class Scene;
class SceneManager
{
	friend class App;
	DECLARE_SINGLE(SceneManager)
private:
	static constexpr int m_iChangeSignal = 0xabcdef;
	int m_iSignal = 0;
	SCENE_CREATE m_eNext = SC_END;
	STAGE_SHOW m_eCurShowMode = SHOW_ALL;
	vector<Scene*> m_vecScene;
	Scene* m_pScene;
	void ChangeScene();
	void ChangeShowMode();
private:
	static constexpr float m_fSceneDelay = 1.0f;
	static constexpr float m_fSceneDrawPeriod = 0.04f;
	float m_fDelay = 0.f;
	void FadeOut();
public:
	STAGE_SHOW GetShowMode()const { return m_eCurShowMode; }
	bool NeedChangeScene(int result) { return result == m_iChangeSignal; }
	void SignalizeSceneChange(SCENE_CREATE next);
	class Scene* GetScene() const { return m_pScene; }
public:
	bool Init();
	void Input(float dt);
	int Update(float dt);
	int LateUpdate(float dt);
	void Collision(float dt);
	void Draw(HDC hdc, float dt);
	template<typename T>
	T* CreateScene(SCENE_CREATE sc)
	{
		T* pScene = new T;

		if (m_vecScene[sc])
		{
			SAFE_DELETE(m_vecScene[sc]);
		}

		pScene->SetSceneType(sc);

		if (!pScene->Init())
		{
			SAFE_DELETE(pScene);
			return nullptr;
		}

		m_vecScene[sc] = pScene;
		return pScene;
	}
};


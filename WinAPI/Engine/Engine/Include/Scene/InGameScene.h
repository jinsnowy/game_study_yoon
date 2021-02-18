#pragma once
#include "Scene.h"
class InGameScene : public Scene
{
private:
	friend class SceneManager;

private:
	InGameScene();
	~InGameScene();
public:
	virtual bool Init();
};


#pragma once
#include "Scene.h"
class InGameScene : public Scene
{
private:
	friend class CSceneManager;

private:
	InGameScene();
	~InGameScene();
public:
	virtual bool Init();
};


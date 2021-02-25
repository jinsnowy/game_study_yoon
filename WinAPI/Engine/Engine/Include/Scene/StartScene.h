#pragma once
#include "Scene.h"
class StartScene :
    public Scene
{
private:
    friend class SceneManager;
private:
    StartScene();
    ~StartScene();
public:
    virtual bool Init();
};
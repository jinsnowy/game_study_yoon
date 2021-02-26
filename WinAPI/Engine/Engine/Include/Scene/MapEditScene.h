#pragma once
#include "Scene.h"

class MapEditScene :
    public Scene
{
private:
    friend class SceneManager;
private:
    MapEditScene();
    ~MapEditScene();
public:
    virtual bool Init();
    virtual void Input(float dt);
public:
    void StartButtonCallback(float fTime);
    void EndButtonCallback(float fTime);
    void EditButtonCallback(float fTime);
};
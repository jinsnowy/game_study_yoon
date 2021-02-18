#include "InGameScene.h"

InGameScene::InGameScene()
{
}

InGameScene::~InGameScene()
{
}

bool InGameScene::Init()
{
    if (!Scene::Init())
    {
        return false;
    }
    return true;
}

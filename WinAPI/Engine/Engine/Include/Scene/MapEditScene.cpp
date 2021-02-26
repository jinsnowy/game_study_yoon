#include "StartScene.h"
#include "SceneManager.h"
#include "Layer.h"
#include "../Application/Window.h"
#include "../Object/StaticObj/Stage.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Camera.h"
#include "MapEditScene.h"

MapEditScene::MapEditScene()
{
}

MapEditScene::~MapEditScene()
{
}

bool MapEditScene::Init()
{
    if (!Scene::Init())
    {
        return false;
    }

    CAMERA->SetWorldResolution(2000.f, 2000.f);
    CAMERA->SetPos(0.f, 0.f);
    CAMERA->SetPivot(0.f, 0.f);
    CAMERA->ReleaseTarget();

    Layer* pStageLayer = FindLayer("Stage");

    Stage* pStage = Object::CreateObject<Stage>("Stage", pStageLayer);

    pStage->CreateTile(25, 79, 16, 16, "BaseTile", L"GameTiles.bmp");

    SAFE_RELEASE(pStage);
    return true;
}

void MapEditScene::Input(float dt)
{
    Scene::Input(dt);

    if (GetAsyncKeyState('W') & 0x8000)
    {
        CAMERA->Scroll(0.f, -300.f * dt);
    }
    if (GetAsyncKeyState('S') & 0x8000)
    {
        CAMERA->Scroll(0.f, 300.f * dt);
    }
    if (GetAsyncKeyState('A') & 0x8000)
    {
        CAMERA->Scroll(-300.f * dt, 0.f);
    }
    if (GetAsyncKeyState('D') & 0x8000)
    {
        CAMERA->Scroll(300.f * dt, 0.f);
    }
}

void MapEditScene::StartButtonCallback(float fTime)
{
}

void MapEditScene::EndButtonCallback(float fTime)
{
}

void MapEditScene::EditButtonCallback(float fTime)
{
}

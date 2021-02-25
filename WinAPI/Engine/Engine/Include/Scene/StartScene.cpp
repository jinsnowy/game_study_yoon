#include "StartScene.h"
#include "../Object/Stage.h"
#include "../Collider/ColliderPixel.h"
#include "../Application/Window.h"
#include "../Core/Camera.h"
#include "../Object/UIPanel.h"
#include "Layer.h"

StartScene::StartScene()
{
}

StartScene::~StartScene()
{
}

bool StartScene::Init()
{
    if (!Scene::Init())
    {
        return false;
    }

    Layer* pLayer = FindLayer("UI");

    UIPanel* pBackPanel = Object::CreateObject<UIPanel>("BackPanel", pLayer);

    pBackPanel->SetSize(GETRESOLUTION.x, GETRESOLUTION.y);
    pBackPanel->SetTexture("StartBack", L"MainMenu.bmp");

    SAFE_RELEASE(pBackPanel);

	return true;
}

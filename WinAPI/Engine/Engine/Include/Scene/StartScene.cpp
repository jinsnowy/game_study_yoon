#include "StartScene.h"
#include "../Object/Stage.h"
#include "../Application/Window.h"
#include "../Core/Camera.h"
#include "../Object/UIPanel.h"
#include "../Object/UIButton.h"
#include "../Collider/ColliderRect.h"
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

    UIButton* pStartBtn = Object::CreateObject<UIButton>("StartButtton", pLayer);

    pStartBtn->SetPos(GETRESOLUTION.x/2 - 100, GETRESOLUTION.y/2 + 20);
    pStartBtn->SetSize(200, 100);
    pStartBtn->SetTexture("StartButtton", L"UIButton.bmp");

    ColliderRect* pRC = static_cast<ColliderRect*>(pStartBtn->GetCollider("ButtonBody"));
    
    Pos tPos = pStartBtn->GetPos();
    Size tSize = pStartBtn->GetSize();
    pRC->SetRect(tPos.x, tPos.y, tPos.x + tSize.x, tPos.y + tSize.y);

    SAFE_RELEASE(pRC);

    SAFE_RELEASE(pStartBtn);

	return true;
}

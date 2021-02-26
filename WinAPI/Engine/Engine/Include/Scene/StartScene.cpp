#include "StartScene.h"
#include "SceneManager.h"
#include "Layer.h"
#include "InGameScene.h"
#include "../Application/Window.h"
#include "../Object/Stage.h"
#include "../Application/Window.h"
#include "../Core/Camera.h"
#include "../Object/UIPanel.h"
#include "../Object/UIButton.h"
#include "../Collider/ColliderRect.h"


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

    Layer* pLayer = FindLayer("Stage");

    UIPanel* pBackPanel = Object::CreateObject<UIPanel>("BackPanel", pLayer);

    pBackPanel->SetSize(GETRESOLUTION.x, GETRESOLUTION.y);
    pBackPanel->SetTexture("StartBack", L"StardewValley_TitleScreen_BitMap.bmp");

    SAFE_RELEASE(pBackPanel);

    pLayer = FindLayer("UI");
    UIPanel* pGameTitle = Object::CreateObject<UIPanel>("GameTitle", pLayer);
    pGameTitle->SetPos(GETRESOLUTION.x / 2.f, 200.f);
    pGameTitle->SetSize(480.f, 223.f);
    pGameTitle->SetPivot(0.5f, 0.5f);
    pGameTitle->SetTexture("GameTitle", L"NameTitle.bmp");
    pGameTitle->SetColorKey(255, 255, 255);
    SAFE_RELEASE(pGameTitle);

    UIButton* pStartBtn = Object::CreateObject<UIButton>("NewButton", pLayer);
    pStartBtn->SetPos(GETRESOLUTION.x/2 - 100, GETRESOLUTION.y/2 + 20);
    pStartBtn->SetSize(74.f, 58.f);
    pStartBtn->SetTexture("NewButton", L"StardewValley_TitleScreen_SpriteSheet.bmp");
    pStartBtn->SetImageOffset(0, 187.f);
    pStartBtn->SetMouseOutImageOffset(pStartBtn->GetImageOffset());
    pStartBtn->SetMouseOnImageOffset(0, 187.f + pStartBtn->GetSize().y);
    pStartBtn->SetColorKey(255, 255, 255);

    ColliderRect* pRC = static_cast<ColliderRect*>(pStartBtn->GetCollider("ButtonBody"));
    Size tSize = pStartBtn->GetSize();
    pRC->SetRect(0.f, 0.f, tSize.x, tSize.y);
    pRC->AddCollisionFunction(CS_ENTER, pStartBtn, &UIButton::MouseOn);
    pRC->AddCollisionFunction(CS_LEAVE, pStartBtn, &UIButton::MouseOut);
    pRC->EnableUICollider(true);
    SAFE_RELEASE(pRC);

    pStartBtn->SetCallback(this, &StartScene::StartButtonCallback);
    SAFE_RELEASE(pStartBtn);

	return true;
}

void StartScene::StartButtonCallback(float fTime)
{
    SCENE_MANAGER->CreateScene<InGameScene>(SC_NEXT);
}

void StartScene::EndButtonCallback(float fTime)
{
    WINDOW->DestroyGame();
}

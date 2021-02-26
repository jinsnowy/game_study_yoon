#include "StartScene.h"
#include "SceneManager.h"
#include "Layer.h"
#include "InGameScene.h"
#include "../Application/Window.h"
#include "../Object/StaticObj/Stage.h"
#include "../Application/Window.h"
#include "../Core/Camera.h"
#include "../Object/StaticObj/UIPanel.h"
#include "../Object/StaticObj/UIButton.h"
#include "../Collider/ColliderRect.h"
#include "MapEditScene.h"


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

    float btnWidth = 74.f;
    float btnHeight = 58.f;
    UIButton* pStartBtn = Object::CreateObject<UIButton>("NewButton", pLayer);
    pStartBtn->SetPos(GETRESOLUTION.x/2 - 2 * btnWidth, GETRESOLUTION.y/2 + 50);
    pStartBtn->SetSize(btnWidth, btnHeight);
    pStartBtn->SetTexture("NewButton", L"StardewValley_TitleScreen_SpriteSheet.bmp");
    pStartBtn->SetImageOffset(0, 187.f);
    pStartBtn->SetMouseOutImageOffset(pStartBtn->GetImageOffset());
    pStartBtn->SetMouseOnImageOffset(0, 187.f + pStartBtn->GetSize().y);
    pStartBtn->SetColorKey(255, 255, 255);

    ColliderRect* pRC = static_cast<ColliderRect*>(pStartBtn->GetCollider("ButtonBody"));
    Size tSize = pStartBtn->GetSize();
    pRC->SetRect(0.f, 0.f, tSize.x, tSize.y);
    SAFE_RELEASE(pRC);
    pStartBtn->SetCallback(this, &StartScene::StartButtonCallback);
    SAFE_RELEASE(pStartBtn);

    UIButton* pEditBtn = Object::CreateObject<UIButton>("EditButton", pLayer);
    pEditBtn->SetPos(GETRESOLUTION.x / 2 - btnWidth, GETRESOLUTION.y / 2 + 50);
    pEditBtn->SetSize(btnWidth, btnHeight);
    pEditBtn->SetTexture("EditButton", L"StardewValley_TitleScreen_SpriteSheet.bmp");
    pEditBtn->SetImageOffset(btnWidth, 187.f);
    pEditBtn->SetMouseOutImageOffset(pEditBtn->GetImageOffset());
    pEditBtn->SetMouseOnImageOffset(btnWidth, 187.f + pEditBtn->GetSize().y);
    pEditBtn->SetColorKey(255, 255, 255);

    pRC = static_cast<ColliderRect*>(pEditBtn->GetCollider("ButtonBody"));
    tSize = pEditBtn->GetSize();
    pRC->SetRect(0.f, 0.f, tSize.x, tSize.y);
    SAFE_RELEASE(pRC);
    pEditBtn->SetCallback(this, &StartScene::EditButtonCallback);
    SAFE_RELEASE(pEditBtn);

    // 종료 버튼
    UIButton* pEndBtn = Object::CreateObject<UIButton>("ExitButton", pLayer);
    pEndBtn->SetPos(GETRESOLUTION.x / 2, GETRESOLUTION.y / 2 + 50);
    pEndBtn->SetSize(btnWidth, btnHeight);
    pEndBtn->SetTexture("ExitButton", L"StardewValley_TitleScreen_SpriteSheet.bmp");
    pEndBtn->SetImageOffset(2 * btnWidth, 187.f);
    pEndBtn->SetMouseOutImageOffset(pEndBtn->GetImageOffset());
    pEndBtn->SetMouseOnImageOffset(2 * btnWidth, 187.f + pEndBtn->GetSize().y);
    pEndBtn->SetColorKey(255, 255, 255);

    pRC = static_cast<ColliderRect*>(pEndBtn->GetCollider("ButtonBody"));
    tSize = pEndBtn->GetSize();
    pRC->SetRect(0.f, 0.f, tSize.x, tSize.y);
    SAFE_RELEASE(pRC);
    pEndBtn->SetCallback(this, &StartScene::EndButtonCallback);
    SAFE_RELEASE(pEndBtn);

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

void StartScene::EditButtonCallback(float fTime)
{
    SCENE_MANAGER->CreateScene<MapEditScene>(SC_NEXT);
}

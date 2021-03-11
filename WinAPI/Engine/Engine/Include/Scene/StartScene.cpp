#include "StartScene.h"
#include "SceneManager.h"
#include "Layer.h"
#include "InHouseScene.h"
#include "../Application/Window.h"
#include "../Object/StaticObj/Stage.h"
#include "../Application/Window.h"
#include "../Core/Camera.h"
#include "../Object/StaticObj/UIPanel.h"
#include "../Object/StaticObj/UIButton.h"
#include "../Collider/ColliderRect.h"
#include "MapEditScene.h"
#include "../Sound/SoundManager.h"


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

    // 사운드 로드
    SOUND_MANAGER->LoadSound("StartScene_BGM", true, SD_BACKGROUND, "BGM_StartScene_Long.mp3");
    SOUND_MANAGER->LoadSound("StartScene_Click", false, SD_EFFECT, "ClickButton_StartScene.mp3");
    SOUND_MANAGER->LoadSound("StartScene_On", false, SD_EFFECT, "ClickButtonOn_StartScene.mp3");

    Layer* pLayer = FindLayer("Ground");
    UIPanel* pBackPanel = CreateObject<UIPanel>("BackPanel", pLayer);

    pBackPanel->SetSize(GETRESOLUTION.x, GETRESOLUTION.y);
    pBackPanel->SetTexture("StartBack", L"StartScene/StardewValley_TitleScreen_BitMap.bmp");

    SAFE_RELEASE(pBackPanel);

    pLayer = FindLayer("UI");
    UIPanel* pGameTitle = CreateObject<UIPanel>("GameTitle", pLayer);
    pGameTitle->SetPos(GETRESOLUTION.x / 2.f, 200.f);
    pGameTitle->SetSize(480.f, 223.f);
    pGameTitle->SetPivot(0.5f, 0.5f);
    pGameTitle->SetTexture("GameTitle", L"StartScene/NameTitle.bmp");
    pGameTitle->SetColorKey(255, 255, 255);
    SAFE_RELEASE(pGameTitle);

    float btnWidth = 120.f;
    float btnHeight = 93.f;

    // 시작 버튼
    UIButton* pStartBtn = CreateObject<UIButton>("NewButton", pLayer);
    pStartBtn->SetPos(GETRESOLUTION.x/2 - 2 * btnWidth, GETRESOLUTION.y/2 + 50);
    pStartBtn->SetSize(btnWidth, btnHeight);
    pStartBtn->SetTexture("NewButton", L"StartScene/SelectScene.bmp");
    pStartBtn->SetImageOffset(0.f, 0.f);
    pStartBtn->SetMouseOnOutImage(true);
    pStartBtn->SetMouseOutImageOffset(pStartBtn->GetImageOffset());
    pStartBtn->SetMouseOnImageOffset(pStartBtn->GetImageOffset().x,
                                    btnHeight + pStartBtn->GetImageOffset().y);
    pStartBtn->SetColorKey(255, 255, 255);
    pStartBtn->SetSoundTag("StartScene_On");


    ColliderRect* pRC = static_cast<ColliderRect*>(pStartBtn->GetCollider("ButtonBody"));
    Size tSize = pStartBtn->GetSize();
    pRC->SetRect(0.f, 0.f, tSize.x, tSize.y);
    SAFE_RELEASE(pRC);
    pStartBtn->SetCallback(this, &StartScene::StartButtonCallback);
    SAFE_RELEASE(pStartBtn);

    // 편집 버튼
    UIButton* pEditBtn = CreateObject<UIButton>("EditButton", pLayer);
    pEditBtn->SetPos(GETRESOLUTION.x / 2 - btnWidth, GETRESOLUTION.y / 2 + 50);
    pEditBtn->SetSize(btnWidth, btnHeight);
    pEditBtn->SetTexture("EditButton", L"StartScene/SelectScene.bmp");
    pEditBtn->SetImageOffset(btnWidth, 0.f);
    pEditBtn->SetMouseOnOutImage(true);
    pEditBtn->SetMouseOutImageOffset(pEditBtn->GetImageOffset());
    pEditBtn->SetMouseOnImageOffset(pEditBtn->GetImageOffset().x,
                                    btnHeight + pEditBtn->GetImageOffset().y);
    pEditBtn->SetColorKey(255, 255, 255);
    pEditBtn->SetSoundTag("StartScene_On");

    pRC = static_cast<ColliderRect*>(pEditBtn->GetCollider("ButtonBody"));
    tSize = pEditBtn->GetSize();
    pRC->SetRect(0.f, 0.f, tSize.x, tSize.y);
    SAFE_RELEASE(pRC);
    pEditBtn->SetCallback(this, &StartScene::EditButtonCallback);
    SAFE_RELEASE(pEditBtn);

    // Op
    UIButton* pOpBtn = CreateObject<UIButton>("NoneButton", pLayer);
    pOpBtn->SetPos(GETRESOLUTION.x / 2, GETRESOLUTION.y / 2 + 50);
    pOpBtn->SetSize(btnWidth, btnHeight);
    pOpBtn->SetTexture("ExitButton", L"StartScene/SelectScene.bmp");
    pOpBtn->SetImageOffset(2 * btnWidth, 0.f);
    pOpBtn->SetMouseOnOutImage(true);
    pOpBtn->SetMouseOutImageOffset(pOpBtn->GetImageOffset());
    pOpBtn->SetMouseOnImageOffset(pOpBtn->GetImageOffset().x,
        btnHeight + pOpBtn->GetImageOffset().y);
    pOpBtn->SetColorKey(255, 255, 255);
    pOpBtn->SetSoundTag("StartScene_On");
    SAFE_RELEASE(pOpBtn);

    // 종료 버튼
    UIButton* pEndBtn = CreateObject<UIButton>("ExitButton", pLayer);
    pEndBtn->SetPos(GETRESOLUTION.x / 2 + btnWidth, GETRESOLUTION.y / 2 + 50);
    pEndBtn->SetSize(btnWidth, btnHeight);
    pEndBtn->SetTexture("ExitButton", L"StartScene/SelectScene.bmp");
    pEndBtn->SetImageOffset(3 * btnWidth, 0.f);
    pEndBtn->SetMouseOnOutImage(true);
    pEndBtn->SetMouseOutImageOffset(pEndBtn->GetImageOffset());
    pEndBtn->SetMouseOnImageOffset(pEndBtn->GetImageOffset().x,
                                    btnHeight + pEndBtn->GetImageOffset().y);
    pEndBtn->SetColorKey(255, 255, 255);
    pEndBtn->SetSoundTag("StartScene_On");

    pRC = static_cast<ColliderRect*>(pEndBtn->GetCollider("ButtonBody"));
    tSize = pEndBtn->GetSize();
    pRC->SetRect(0.f, 0.f, tSize.x, tSize.y);
    SAFE_RELEASE(pRC);
    pEndBtn->SetCallback(this, &StartScene::EndButtonCallback);
    SAFE_RELEASE(pEndBtn);

    SOUND_MANAGER->PlaySound("StartScene_BGM");
	return true;
}

void StartScene::StartButtonCallback(float fTime)
{
    SOUND_MANAGER->PlaySound("StartScene_Click");
    SOUND_MANAGER->PauseSound(SD_BACKGROUND);

    SceneState nxt;
    nxt.nextDir = LEFT;
    nxt.nextBeacon = BC_TWO;
    nxt.nextScene = SC_INHOUSE;
    SCENE_MANAGER->SignalizeSceneChange(nxt);
}

void StartScene::EndButtonCallback(float fTime)
{
    SOUND_MANAGER->PlaySound("StartScene_Click");
    SOUND_MANAGER->PauseSound(SD_BACKGROUND);
    WINDOW->DestroyGame();
}

void StartScene::EditButtonCallback(float fTime)
{
    SceneState nxt;
    nxt.nextDir = RIGHT;
    nxt.nextBeacon = BC_NONE;
    nxt.nextScene = SC_MAPEDIT;
    SOUND_MANAGER->PlaySound("StartScene_Click");
    SOUND_MANAGER->PauseSound(SD_BACKGROUND);
    SCENE_MANAGER->SignalizeSceneChange(nxt);
}

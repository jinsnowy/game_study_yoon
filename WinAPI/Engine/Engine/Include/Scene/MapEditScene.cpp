#include "MapEditScene.h"
#include "StartScene.h"
#include "SceneManager.h"
#include "Layer.h"
#include "../Application/Window.h"
#include "../Object/StaticObj/Stage.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Camera.h"
#include "../Core/Input.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "../Resource.h"
#include "../Core/PathManager.h"

wchar_t MapEditScene::m_strText[MAX_PATH] = {};

MapEditScene::MapEditScene()
{
    m_eTem = TEM_TEXTURE;
    m_iEditTileTex = 0;
    m_eEditOption = TO_NONE;
}

MapEditScene::~MapEditScene()
{
    Safe_Release_VecList(m_vecTileTex);
    SAFE_RELEASE(m_pStage);
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

    Texture* pNoneTex = RESOURCE_MANAGER->LoadTexture("TileNoMove", L"NoMove.bmp");
    pNoneTex->SetColorKey(255, 0, 255);
    SAFE_RELEASE(pNoneTex);

    Texture* pNoMoveTex = RESOURCE_MANAGER->LoadTexture("TileNone", L"NoOption.bmp");
    pNoMoveTex->SetColorKey(255, 0, 255);
    SAFE_RELEASE(pNoMoveTex);

    Layer* pStageLayer = FindLayer("Stage");

    m_pStage = Object::CreateObject<Stage>("Stage", pStageLayer);
    m_pStage->CreateTile(100, 100, 45, 45, "BaseTile", L"BaseTile.bmp");

    Texture* pTex = RESOURCE_MANAGER->LoadTexture("Tile1", L"Sample.bmp");
    m_vecTileTex.push_back(pTex);
    pTex = RESOURCE_MANAGER->LoadTexture("Tile2", L"Sample2.bmp");
    m_vecTileTex.push_back(pTex);
    pTex = RESOURCE_MANAGER->FindTexture("BaseTile");
    m_vecTileTex.push_back(pTex);

    m_iEditTileTex = 0;
    m_eEditOption = TO_NONE;

    INPUT->AddKey("Save", VK_CONTROL, 'S');
    INPUT->AddKey("Load", VK_CONTROL, 'O');
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
    if (GetAsyncKeyState(VK_F1) & 0x8000)
    {
        m_eTem = TEM_TEXTURE;
    }
    if (GetAsyncKeyState(VK_F2) & 0x8000)
    {
        m_eTem = TEM_OPTION;
    }
    if (GetAsyncKeyState('1') & 0x8000)
    {
        switch (m_eTem)
        {
        case TEM_TEXTURE:
            m_iEditTileTex = 0;
            break;
        case TEM_OPTION:
            m_eEditOption = TO_NONE;
            break;
        }
    }
    if (GetAsyncKeyState('2') & 0x8000)
    {
        switch (m_eTem)
        {
        case TEM_TEXTURE:
            m_iEditTileTex = 1;
            break;
        case TEM_OPTION:
            m_eEditOption = TO_NOMOVE;
            break;
        }
    }
    if (GetAsyncKeyState('3') & 0x8000)
    {
        switch (m_eTem)
        {
        case TEM_TEXTURE:
            m_iEditTileTex = 2;
            break;
        case TEM_OPTION:
            m_eEditOption = TO_NONE;
            break;
        }
    }
    if (KEYPRESS("MouseLButton"))
    {
        Pos tMouseWorldPos = MOUSEWORLDPOS;
        switch (m_eTem)
        {
        case TEM_TEXTURE:
            m_pStage->ChangeTileTexture(tMouseWorldPos, m_vecTileTex[m_iEditTileTex]);
            break;
        case TEM_OPTION:
            m_pStage->ChangeTileOption(tMouseWorldPos, m_eEditOption);
            break;
        }
    }
    if (KEYPRESS("MouseRButton"))
    {

    }

    if (KEYDOWN("Save"))
    {
        ShowCursor(TRUE);
        DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1), WINDOWHANDLE, MapEditScene::DlgProc);
        ShowCursor(FALSE);

        // 파일명을 이용하여 저장한다.
        char strFileName[MAX_PATH] = {};
        WideCharToMultiByte(CP_ACP, 0, m_strText, -1, strFileName, lstrlen(m_strText), 0, 0);

        m_pStage->SaveFromPath(strFileName);
    }

    if (KEYDOWN("Load"))
    {
        ShowCursor(TRUE);
        DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1), WINDOWHANDLE, MapEditScene::DlgProc);
        ShowCursor(FALSE);

        // 파일명을 이용하여 읽어온다.
        char strFileName[MAX_PATH] = {};
        WideCharToMultiByte(CP_ACP, 0, m_strText, -1, strFileName, lstrlen(m_strText), 0, 0);
        
        if (!m_pStage)
        {
            Layer* pStageLayer = FindLayer("Stage");
            m_pStage = Object::CreateObject<Stage>("Stage", pStageLayer);
        }
        m_pStage->LoadFromPath(strFileName);
    }
}


INT_PTR MapEditScene::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
            // Edit Box에서 문자열을 얻어온다.
            memset(m_strText, 0, sizeof(wchar_t) * MAX_PATH);
            GetDlgItemText(hWnd, IDC_EDIT1, m_strText, MAX_PATH);
        case IDCANCEL:
            EndDialog(hWnd, IDOK);
            return TRUE;
        }
        return FALSE;
    }

    return FALSE;
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

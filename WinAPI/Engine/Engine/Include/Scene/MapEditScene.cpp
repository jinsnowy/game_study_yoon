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
#include "../Sound/SoundManager.h"
#include "../Object/StaticObj/UIButton.h"
#include "../Object/StaticObj/UIPanel.h"
#include "../Object/StaticObj/UITileSelect.h"

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
    SAFE_RELEASE(m_pSelUI);
    SAFE_RELEASE(m_pSelTexture);
}

bool MapEditScene::Init()
{
    if (!Scene::Init())
    {
        return false;
    }

    SetUpCamera();

    SetUpBaseTiles();

    SetUpBackButton();

    SetUpTileSelectUI();

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
        Pos tMouseClientPos = MOUSECLIENTPOS;
        Pos tMouseWorldPos = MOUSEWORLDPOS;
        ColliderRect* rc = static_cast<ColliderRect*>(Object::FindObject("BackButton")->GetCollider("ButtonBody"));
        Rect bound = rc->GetWorldInfo();
        SAFE_RELEASE(rc);

        // 뒤로가기 버튼 체크 먼저
        if (bound.ContainsPoint(tMouseClientPos))
        {
            BackButtonCallback();
        }
        Texture* selTex = m_pSelUI->SelectTile(tMouseClientPos);
        if (selTex)
        {
            SAFE_RELEASE(m_pSelTexture);
            m_pSelTexture = selTex;
        }
        else {
            switch (m_eTem)
            {
            case TEM_TEXTURE:
                if (m_pSelTexture)
                {
                    m_pStage->ChangeTileTexture(tMouseWorldPos, m_pSelTexture);
                }
                break;
            case TEM_OPTION:
                m_pStage->ChangeTileOption(tMouseWorldPos, m_eEditOption);
                break;
            }
        }
    }

    if (KEYPRESS("MouseRButton"))
    {
        Pos tMouseWorldPos = MOUSEWORLDPOS;
        switch (m_eTem)
        {
        case TEM_TEXTURE:
            if (m_pSelTexture)
            {
                m_pStage->ChangeTileTexture(tMouseWorldPos, m_pStage->GetBaseTexture());
            }
            break;
        case TEM_OPTION:
            m_pStage->ChangeTileOption(tMouseWorldPos, m_eEditOption);
            break;
        }
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

void MapEditScene::BackButtonCallback()
{
    SOUND_MANAGER->PlaySound("StartScene_Click");
    SCENE_MANAGER->CreateScene<StartScene>(SC_NEXT);
}


void MapEditScene::Draw(HDC hDC, float dt)
{
    Scene::Draw(hDC, dt);

    Pos tPos = MOUSEWORLDPOS;
    Pos tCamPos = CAMERA->GetTopLeft();
    RESOLUTION tileSize = m_pStage->GetTileSize();

    int left = int(tPos.x / tileSize.x) * tileSize.x - tCamPos.x;
    int top = int(tPos.y / tileSize.y) * tileSize.y - tCamPos.y;
    int right = left + tileSize.x;
    int bottom = top + tileSize.y;

    HPEN myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    HPEN OldPen = (HPEN)SelectObject(hDC, myPen);
    HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

    Rectangle(hDC, left, top, right, bottom);
    DeleteObject(SelectObject(hDC, OldPen));
    DeleteObject(SelectObject(hDC, OldBrush));

    if (m_pSelTexture)
    {
        m_pSelTexture->TileDraw(hDC,
                                MOUSECLIENTPOS.x - TILESIZE / 2,
                                MOUSECLIENTPOS.y - TILESIZE / 2);
    }
}

void MapEditScene::SetUpCamera()
{
    CAMERA->SetWorldResolution(2000.f, 2000.f);
    CAMERA->SetPos(0.f, 0.f);
    CAMERA->SetPivot(0.f, 0.f);
    CAMERA->ReleaseTarget();
}

void MapEditScene::SetUpBaseTiles()
{
    Texture* pNoneTex = RESOURCE_MANAGER->LoadTexture("TileNoMove", L"NoMove.bmp");
    pNoneTex->SetColorKey(255, 0, 255);
    SAFE_RELEASE(pNoneTex);

    Texture* pNoMoveTex = RESOURCE_MANAGER->LoadTexture("TileNone", L"NoOption.bmp");
    pNoMoveTex->SetColorKey(255, 0, 255);
    SAFE_RELEASE(pNoMoveTex);

    Layer* pStageLayer = FindLayer("Stage");
    m_pStage = Object::CreateObject<Stage>("Stage", pStageLayer);
    m_pStage->CreateTile(100, 100, 64, 64, "BaseTile", L"SV/BaseTile.bmp");
}

void MapEditScene::SetUpBackButton()
{
    Layer* pLayer = FindLayer("UI");
    UIButton* pBackBtn = Object::CreateObject<UIButton>("BackButton", pLayer);
    pBackBtn->SetPos(30, GETRESOLUTION.y - 100);
    pBackBtn->SetSize(216.f, 88.f);
    pBackBtn->SetTexture("BackButton", L"SV/BackButton.bmp");
    pBackBtn->SetImageOffset(0.f, 0.f);
    pBackBtn->SetMouseOutImageOffset(pBackBtn->GetImageOffset());
    pBackBtn->SetMouseOnImageOffset(pBackBtn->GetImageOffset().x,
                                    88.f + pBackBtn->GetImageOffset().y);
    pBackBtn->SetColorKey(255, 255, 255);
    pBackBtn->SetSoundTag("StartScene_On");

    ColliderRect* pRC = static_cast<ColliderRect*>(pBackBtn->GetCollider("ButtonBody"));
    Size tSize = pBackBtn->GetSize();
    pRC->SetRect(0.f, 0.f, tSize.x, tSize.y);
    SAFE_RELEASE(pRC);
    SAFE_RELEASE(pBackBtn);
}


void MapEditScene::SetUpTileSelectUI()
{
    Layer* pLayer = Scene::FindLayer("UI");

    Texture* pTex = RESOURCE_MANAGER->LoadTexture("Ground", L"SV/UISelectBase2.bmp");

    m_pSelUI = Object::CreateObject<UITileSelect>("SelectUI", pLayer);

    // Ground 타일들
    m_pSelUI->LoadTiles(UITileSelect::UISEL_TYPE::SEL_GROUND, L"SV/Ground/");
    // 선택 번호 타일들
    m_pSelUI->LoadTiles(UITileSelect::UISEL_TYPE::SEL_NUMBER, L"SV/Numbers/Select/");

    m_pSelUI->SetTexture(pTex);
    m_pSelUI->SetSize(pTex->GetWidth(), pTex->GetHeight());
    m_pSelUI->SetPos(GETRESOLUTION.x - pTex->GetWidth() - 50, 200.f);

    SAFE_RELEASE(pTex);
}

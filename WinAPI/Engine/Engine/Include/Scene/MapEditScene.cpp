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
    m_vecStage.resize(ST_END);
}

MapEditScene::~MapEditScene()
{
    Safe_Release_VecList(m_vecStage);
    SAFE_RELEASE(m_pSelUI);
    SAFE_RELEASE(m_pSelTexture);
    SAFE_RELEASE(m_pSelButton);
}

bool MapEditScene::Init()
{
    if (!Scene::Init())
    {
        return false;
    }

    Texture* pNoneTex = RESOURCE_MANAGER->LoadTexture("TileNoMove", L"NoMove.bmp");
    pNoneTex->SetColorKey(255, 0, 255);
    SAFE_RELEASE(pNoneTex);

    Texture* pNoMoveTex = RESOURCE_MANAGER->LoadTexture("TileNone", L"NoOption.bmp");
    pNoMoveTex->SetColorKey(255, 0, 255);
    SAFE_RELEASE(pNoMoveTex);

    SetUpCamera();

    SetUpDefaultStages(50, 50);

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

    if (KEYPRESS("MouseLButton"))
    {
        Pos tMouseClientPos = MOUSECLIENTPOS;
        Pos tMouseWorldPos = MOUSEWORLDPOS;
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
                    m_vecStage[m_eCurStage]->ChangeTileTexture(tMouseWorldPos, m_pSelTexture);
                }
                break;
            }
        }
    }

    if (KEYPRESS("MouseRButton"))
    {
        Pos tMouseWorldPos = MOUSEWORLDPOS;
        SAFE_RELEASE(m_pSelTexture);
        switch (m_eTem)
        {
        case TEM_TEXTURE:
            m_vecStage[m_eCurStage]->SetTileNone(tMouseWorldPos);
            break;
        case TEM_OPTION:
            m_vecStage[m_eCurStage]->ChangeTileOption(tMouseWorldPos, m_eEditOption);
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

        SaveDefaultStages(strFileName);
    }

    if (KEYDOWN("Load"))
    {
        ShowCursor(TRUE);
        DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1), WINDOWHANDLE, MapEditScene::DlgProc);
        ShowCursor(FALSE);

        // 파일명을 이용하여 읽어온다.
        char strFileName[MAX_PATH] = {};
        WideCharToMultiByte(CP_ACP, 0, m_strText, -1, strFileName, lstrlen(m_strText), 0, 0);
        LoadDefaultStages(strFileName);
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

void MapEditScene::Draw(HDC hDC, float dt)
{
    Scene::Draw(hDC, dt);

    // 마우스 선택 타일 드로우
    Pos tPos = MOUSEWORLDPOS;
    Pos tCamPos = CAMERA->GetTopLeft();

    int left = int(tPos.x / TILESIZE) * TILESIZE - tCamPos.x;
    int top = int(tPos.y / TILESIZE) * TILESIZE - tCamPos.y;
    int right = left + TILESIZE;
    int bottom = top + TILESIZE;

    HPEN myPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
    HPEN OldPen = (HPEN)SelectObject(hDC, myPen);
    HBRUSH OldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));

    Rectangle(hDC, left, top, right, bottom);
    DeleteObject(SelectObject(hDC, OldPen));
    DeleteObject(SelectObject(hDC, OldBrush));

    if (m_pSelTexture)
    {
        Size tImgSize = m_pSelTexture->GetSize();
        m_pSelTexture->TileDraw(hDC,
                                MOUSECLIENTPOS.x,
                                MOUSECLIENTPOS.y - tImgSize.y,
                                tImgSize.x, tImgSize.y);
    }
#ifdef _DEBUG
    char buffer[MAX_PATH];
    for (int i = 0; i < ST_END; ++i)
    {
        const string& tileName = m_vecStage[i]->GetTileName(MOUSECLIENTPOS);
        const string& stageName = m_vecStage[i]->GetTag();
        sprintf(buffer, "%s : %s\n", stageName.c_str(), tileName.c_str());
        TextOut(hDC, MOUSECLIENTPOS.x, MOUSECLIENTPOS.y - 100 + 20 * i, GetWChar(buffer), strlen(buffer));
    }
#endif
}

void MapEditScene::SetUpCamera()
{
    CAMERA->SetWorldResolution(2000.f, 2000.f);
    CAMERA->SetPos(0.f, 0.f);
    CAMERA->SetPivot(0.f, 0.f);
    CAMERA->ReleaseTarget();
}

void MapEditScene::SetUpDefaultStages(int numX, int numY)
{
    SetUpBaseStage(ST_GROUND, "Ground", numX, numY);
    SetUpBaseStage(ST_OBJECT, "Object", numX, numY);
    SetUpBaseStage(ST_ONAIR, "OnAir", numX, numY);
    SetUpBaseStage(ST_STATIC, "Static", numX, numY);
}

void MapEditScene::SaveDefaultStages(const char* fileName)
{
    FILE * pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "wb");

    m_vecStage[ST_GROUND]->SaveFromFile(pFile);
    m_vecStage[ST_OBJECT]->SaveFromFile(pFile);
    m_vecStage[ST_ONAIR]->SaveFromFile(pFile);
    m_vecStage[ST_STATIC]->SaveFromFile(pFile);

    if (pFile)
    {
        fclose(pFile);
    }
}

void MapEditScene::SetUpBaseStage(STAGE_TAG eStageTag, const string& strlayerTag, int numX, int numY)
{
    SAFE_RELEASE(m_vecStage[eStageTag]);
    Object::EraseObject(m_vecStage[eStageTag]);
    Layer* pStageLayer = FindLayer(strlayerTag);
    m_vecStage[eStageTag] = Object::CreateObject<Stage>(strlayerTag, pStageLayer);
    m_vecStage[eStageTag]->CreateTile(numX, numY, TILESIZE, TILESIZE, "", L"");
}

void MapEditScene::LoadDefaultStages(const char* fileName)
{
    FILE* pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "rb");

    LoadStage(ST_GROUND, "Ground", pFile);
    LoadStage(ST_OBJECT, "Object", pFile);
    LoadStage(ST_ONAIR, "OnAir", pFile);
    LoadStage(ST_STATIC, "Static", pFile);

    if (pFile)
    {
        fclose(pFile);
    }
}

void MapEditScene::LoadStage(STAGE_TAG eStageTag, const string& strlayerTag, FILE* pFile)
{
    SAFE_RELEASE(m_vecStage[eStageTag]);
    Object::EraseObject(m_vecStage[eStageTag]);
    Layer* pStageLayer = FindLayer(strlayerTag);
    m_vecStage[eStageTag] = Object::CreateObject<Stage>(strlayerTag, pStageLayer);
    m_vecStage[eStageTag]->LoadFromFile(pFile);
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
    pBackBtn->SetCallback(this, &MapEditScene::BackButtonCallback);
    SAFE_RELEASE(pBackBtn);

    // Stage 번호
    for (int i = 0; i < m_btnFileName.size(); ++i)
    {
        wstringstream path;
        path << L"SV/Numbers/Tag/" << m_btnFileName[i] << L".bmp";
        string strKey(GetChar(m_btnFileName[i].c_str()));
        UIButton* pBtn = Object::CreateObject<UIButton>(strKey, pLayer);

        pBtn->SetTexture(strKey, path.str().c_str());
        pBtn->SetSize(120, 60);
        pBtn->SetColorKey(255, 255, 255);
        if (i == 0)
        {
            m_pSelButton = pBtn;
            m_pSelButton->AddRef();
            pBtn->SetImageOffset(120, 0);
        }
        Size tSize = pBtn->GetSize();
        pBtn->SetPos(250.f + i * tSize.x, GETRESOLUTION.y - tSize.y - 30);
        ColliderRect* pRC = static_cast<ColliderRect*>(pBtn->GetCollider("ButtonBody"));
        
        pRC->SetRect(0.f, 0.f, tSize.x, tSize.y);
        SAFE_RELEASE(pRC);
        pBtn->SetCallbackByType(this, pBtn, i, &MapEditScene::StageButtonCallback);
        SAFE_RELEASE(pBtn);
    }
}


void MapEditScene::SetUpTileSelectUI()
{
    Layer* pLayer = Scene::FindLayer("UI");

    Texture* pTex = RESOURCE_MANAGER->LoadTexture("Ground", L"SV/UISelectBase2.bmp");

    m_pSelUI = Object::CreateObject<UITileSelect>("SelectUI", pLayer);

    // Ground 타일들
    m_pSelUI->LoadTiles(UITileSelect::UISEL_TYPE::SEL_GROUND, L"SV/Ground/");
    // 바닥 타일 타일들
    m_pSelUI->LoadTiles(UITileSelect::UISEL_TYPE::SEL_FLOOR, L"SV/FloorTile/");
    // 하우스 내부
    m_pSelUI->LoadTiles(UITileSelect::UISEL_TYPE::SEL_INHOUSE, L"SV/Inhouse/");
    // 선택 번호 타일들
    m_pSelUI->LoadTiles(UITileSelect::UISEL_TYPE::SEL_NUMBER, L"SV/Numbers/Select/");
    // 선택 태그 타일들
    m_pSelUI->LoadTiles(UITileSelect::UISEL_TYPE::SEL_TAG, L"SV/Numbers/Tag/");
    m_pSelUI->SetTexture(pTex);
    m_pSelUI->SetSize(pTex->GetWidth(), pTex->GetHeight());
    m_pSelUI->SetPos(GETRESOLUTION.x - pTex->GetWidth() - 50, 200.f);

    SAFE_RELEASE(pTex);
}

void MapEditScene::BackButtonCallback(float dt)
{
    SOUND_MANAGER->PlaySound("StartScene_Click");
    SCENE_MANAGER->CreateScene<StartScene>(SC_NEXT);
}

void MapEditScene::StageButtonCallback(UIButton* btn, int type, float dt)
{
    STAGE_TAG eTag = static_cast<STAGE_TAG>(type);
    if (m_eCurStage == eTag) return;
    m_eCurStage = eTag;

    switch (eTag)
    {
    case ST_GROUND:
        SCENE_MANAGER->SetShowmode(SHOW_GROUND);
        break;
    case ST_OBJECT:
        SCENE_MANAGER->SetShowmode(SHOW_OBJECT);
        break;
    case ST_ONAIR:
        SCENE_MANAGER->SetShowmode(SHOW_ONAIR);
        break;
    case ST_STATIC:
        SCENE_MANAGER->SetShowmode(SHOW_STATIC);
        break;
    }

    if (m_pSelButton != btn)
    {
        if (m_pSelButton)
        {
            m_pSelButton->SetImageOffset(0, 0);
        }
        SAFE_RELEASE(m_pSelButton);

        if (btn)
        {
            m_pSelButton = btn;
            m_pSelButton->SetImageOffset(120, 0);
            btn->AddRef();
        }
    }
}

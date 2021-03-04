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

wchar_t MapEditScene::m_strText1[MAX_PATH] = {};
wchar_t MapEditScene::m_strText2[MAX_PATH] = {};

MapEditScene::MapEditScene()
{
    m_vecStage.resize(ST_END, nullptr);
    m_eTem = TEM_TEXTURE;
    m_eEditOption = TO_NONE;
}

MapEditScene::~MapEditScene()
{
    INPUT->DeleteKey("SetTexmode");
    INPUT->DeleteKey("SetOptionMode");
    INPUT->DeleteKey("ChangeOption");
    INPUT->DeleteKey("ChangeStage");
    INPUT->DeleteKey("ResetStage");
    INPUT->DeleteKey("Save");
    INPUT->DeleteKey("Load");

    StageClear(ST_GROUND, "Ground");
    StageClear(ST_OBJECT, "Object");
    StageClear(ST_STATIC, "Static");
    
    SAFE_RELEASE(m_pSelUI);
    SAFE_RELEASE(m_pSelTexture);
    Safe_Release_VecList(m_btn);
}

bool MapEditScene::Init()
{
    if (!Scene::Init())
    {
        return false;
    }

    SetUpCamera();

    SetUpDefaultStages(50, 50);

    SetUpUIButton();

    SetUpTileSelectUI();

    LoadOptionTiles(L"SV/Option/");

    INPUT->AddKey("SetTexmode", '1');
    INPUT->AddKey("SetOptionMode", '2');
    INPUT->AddKey("ChangeOption", 'B');
    INPUT->AddKey("ChangeStage", VK_TAB);
    INPUT->AddKey("ResetStage", VK_CONTROL, 'R');
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

    if (KEYDOWN("SetTexMode"))
    {
        m_eTem = TEM_TEXTURE;
        SetSelectTexture(nullptr);
    }
    if (KEYDOWN("SetOptionMode"))
    {
        m_eTem = TEM_OPTION;
        m_eEditOption = static_cast<TILE_OPTION>(TO_NOMOVE);
        Texture* selTex = RESOURCE_MANAGER->FindTexture(m_optTexKey[m_eEditOption]);
        SetSelectTexture(selTex);
    }

    if (KEYDOWN("ChangeOption") && m_eTem == TEM_OPTION)
    {
        m_eEditOption = static_cast<TILE_OPTION>((m_eEditOption + 1) % TO_END);
        Texture* selTex = RESOURCE_MANAGER->FindTexture(m_optTexKey[m_eEditOption]);
        SetSelectTexture(selTex);
    }

    if (KEYPRESS("MouseLButton"))
    {
        Pos tMouseClientPos = MOUSECLIENTPOS;
        Pos tMouseWorldPos = MOUSEWORLDPOS;

        if (m_eTem == TEM_TEXTURE)
        {
            Texture* pTex = m_pSelUI->SelectTile(tMouseClientPos);
            if (pTex)
                SetSelectTexture(pTex);
        }
        if (!m_pSelUI->SelectUITag(tMouseClientPos))
        {
            switch (m_eTem)
            {
            case TEM_TEXTURE:
                 m_vecStage[m_eCurStage]->ChangeTileTexture(tMouseWorldPos, m_pSelTexture);
                break;
            case TEM_OPTION:
                 m_vecStage[m_eCurStage]->ChangeTileOption(tMouseWorldPos, m_eEditOption);
                break;
            }
        }
    }

    if (KEYDOWN("ChangeStage"))
    {
        ChangeStage();
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
            m_vecStage[m_eCurStage]->ChangeTileOption(tMouseWorldPos, TO_NONE);
            break;
        }
    }


    if (KEYDOWN("ResetStage"))
    {
        ShowCursor(TRUE);
        DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG2), WINDOWHANDLE, MapEditScene::DlgProc2);
        ShowCursor(FALSE);

        char strX[10], strY[10];
        WideCharToMultiByte(CP_ACP, 0, m_strText1, -1, strX, 10, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, m_strText2, -1, strY, 10, 0, 0);
        if (strlen(strX) && strlen(strY))
        {
            int sizeX = stoi(string(strX)); int sizeY = stoi(string(strY));
            if (sizeX > 0 && sizeY > 0)
            {
                SetUpDefaultStages(sizeX, sizeY);
            }
        }
    }

    if (KEYDOWN("Save"))
    {
        ShowCursor(TRUE);
        DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1), WINDOWHANDLE, MapEditScene::DlgProc1);
        ShowCursor(FALSE);

        // 파일명을 이용하여 저장한다.
        char strFileName[MAX_PATH] = {};
        WideCharToMultiByte(CP_ACP, 0, m_strText1, -1, strFileName, lstrlen(m_strText1), 0, 0);

        SaveDefaultStages(strFileName);
    }

    if (KEYDOWN("Load"))
    {
        ShowCursor(TRUE);
        DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1), WINDOWHANDLE, MapEditScene::DlgProc1);
        ShowCursor(FALSE);

        // 파일명을 이용하여 읽어온다.
        char strFileName[MAX_PATH] = {};
        WideCharToMultiByte(CP_ACP, 0, m_strText1, -1, strFileName, lstrlen(m_strText1), 0, 0);
        LoadDefaultStages(strFileName);
    }
}

INT_PTR MapEditScene::DlgProc1(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
            memset(m_strText1, 0, sizeof(wchar_t) * MAX_PATH);
            GetDlgItemText(hWnd, IDC_EDIT1, m_strText1, MAX_PATH);
        case IDCANCEL:
            EndDialog(hWnd, IDOK);
            return TRUE;
        }
        return FALSE;
    }

    return FALSE;
}

INT_PTR MapEditScene::DlgProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        memset(m_strText1, 0, sizeof(wchar_t) * MAX_PATH);
        memset(m_strText2, 0, sizeof(wchar_t) * MAX_PATH);
        switch (LOWORD(wParam))
        {
        case IDOK:
            GetDlgItemText(hWnd, IDC_SIZEX, m_strText1, MAX_PATH);
            GetDlgItemText(hWnd, IDC_SIZEY, m_strText2, MAX_PATH);
            EndDialog(hWnd, IDOK);
            return TRUE;
        case IDCANCEL:
            EndDialog(hWnd, IDNO);
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
    stringstream ss;
    for (int i = 0; i < ST_END; ++i)
    {
        const string& tileOption = m_optTexKey[m_vecStage[i]->GetTileOption(MOUSEWORLDPOS)];
        const string& tileName = m_vecStage[i]->GetTileName(MOUSEWORLDPOS);
        const string& stageName = m_vecStage[i]->GetTag();
        ss << std::right << setw(5) << stageName << ": " << std::left << setw(10) << tileName 
            << std::right << setw(5) << ", opt: " << std::left << setw(10) << tileOption << "\n";
        int length = ss.str().size();
        TextOut(hDC, MOUSECLIENTPOS.x, MOUSECLIENTPOS.y - 100 + 20 * i, GetWChar(ss.str().c_str()), length);
        ss.clear();
        ss.str("");
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
    SetUpBaseStage(ST_STATIC, "Static", numX, numY);
}

void MapEditScene::SaveDefaultStages(const char* fileName)
{
    FILE * pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "wb");

    m_vecStage[ST_GROUND]->SaveFromFile(pFile);
    m_vecStage[ST_OBJECT]->SaveFromFile(pFile);
    m_vecStage[ST_STATIC]->SaveFromFile(pFile);

    if (pFile)
    {
        fclose(pFile);
    }
}

void MapEditScene::SetUpBaseStage(STAGE_TAG eStageTag, const string& strlayerTag, int numX, int numY)
{
    StageClear(eStageTag, strlayerTag);
    Layer* pStageLayer = FindLayer(strlayerTag);
    m_vecStage[eStageTag] = Object::CreateObject<Stage>(strlayerTag, pStageLayer);
    m_vecStage[eStageTag]->CreateTile(numX, numY, TILESIZE, TILESIZE);
}

void MapEditScene::LoadDefaultStages(const char* fileName)
{
    FILE* pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "rb");

    LoadStage(ST_GROUND, "Ground", pFile);
    LoadStage(ST_OBJECT, "Object", pFile);
    LoadStage(ST_STATIC, "Static", pFile);

    if (pFile)
    {
        fclose(pFile);
    }
}

void MapEditScene::StageClear(STAGE_TAG eStageTag, const string& layerTag)
{
    if (m_vecStage[eStageTag])
    {
        Layer* pLayer = FindLayer(layerTag);
        assert(pLayer);

        m_vecStage[eStageTag]->ClearTile();
        pLayer->EraseObject(m_vecStage[eStageTag]);
        Object::EraseObject(m_vecStage[eStageTag]);
        SAFE_RELEASE(m_vecStage[eStageTag]);
    }
}

void MapEditScene::LoadStage(STAGE_TAG eStageTag, const string& strlayerTag, FILE* pFile)
{
    StageClear(eStageTag, strlayerTag);
    Layer* pStageLayer = FindLayer(strlayerTag);
    m_vecStage[eStageTag] = Object::CreateObject<Stage>(strlayerTag, pStageLayer);
    m_vecStage[eStageTag]->LoadFromFile(pFile);
}

void MapEditScene::SetUpUIButton()
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

    //// Stage 번호
    m_btn.resize(m_btnFileName.size(), nullptr);
    wstringstream path;
    for (int i = 0; i < m_btnFileName.size(); ++i)
    {
        path << L"SV/Numbers/Tag/" << m_btnFileName[i] << L".bmp";
        string strKey(GetChar(m_btnFileName[i].c_str()));
        m_btn[i] = Object::CreateObject<UIButton>(strKey, pLayer);
        m_btn[i]->SetTexture(strKey, path.str().c_str());
        m_btn[i]->SetSize(120, 60);
        m_btn[i]->SetColorKey(255, 255, 255);
        if (i == 0)
        {
            m_btn[i]->SetImageOffset(120, 0);
        }
        Size tSize = m_btn[i]->GetSize();
        m_btn[i]->SetPos(20+i * tSize.x, 20);
        path.clear();
        path.str(L"");
    }
}


void MapEditScene::SetUpTileSelectUI()
{
    Layer* pLayer = Scene::FindLayer("UI");

    Texture* pTex = RESOURCE_MANAGER->LoadTexture("Ground", L"SV/UISelectBase2.bmp");
    m_pSelUI = Object::CreateObject<UITileSelect>("SelectUI", pLayer);
    m_pSelUI->SetTexture(pTex);
    m_pSelUI->SetSize(pTex->GetWidth(), pTex->GetHeight());
    m_pSelUI->SetPos(GETRESOLUTION.x - pTex->GetWidth() - 50, 200.f);
    SAFE_RELEASE(pTex);

    m_pSelUI->LoadTiles(SEL_GROUND, L"SV/TileGround/");
    m_pSelUI->LoadTiles(SEL_OBJECT1, L"SV/TileObject/");
    m_pSelUI->LoadTiles(SEL_OBJECT2, L"SV/Object/");
    m_pSelUI->LoadTiles(SEL_STATIC, L"SV/TileStatic/");
    m_pSelUI->LoadTiles(SEL_NUMBER, L"SV/Numbers/Select/");
}

void MapEditScene::BackButtonCallback(float dt)
{
    SOUND_MANAGER->PlaySound("StartScene_Click");
    SCENE_MANAGER->CreateScene<StartScene>(SC_NEXT);
}

void MapEditScene::ChangeStage()
{
    m_btn[m_iCurStage]->SetImageOffset(0, 0);
    m_iCurStage = (m_iCurStage + 1) % (ST_END+1);
    m_btn[m_iCurStage]->SetImageOffset(120, 0);

    if (m_iCurStage == 0)
    {
        m_eCurStage = ST_GROUND;
    }
    else if (m_iCurStage == 1 || m_iCurStage == 2)
    {
        m_eCurStage = ST_OBJECT;
    }
    else {
        m_eCurStage = ST_STATIC;
    }
 
    switch (m_iCurStage)
    {
    case 0:
        m_pSelUI->SetCurSelect(SEL_GROUND);
        break;
    case 1:
        m_pSelUI->SetCurSelect(SEL_OBJECT1);
        break;
    case 2:
        m_pSelUI->SetCurSelect(SEL_OBJECT2);
        break;
    case 3:
        m_pSelUI->SetCurSelect(SEL_STATIC);
        break;
    }
}

void MapEditScene::LoadOptionTiles(const wchar_t* pBaseFolderName, const string& strPathKey)
{
 
    const wchar_t* pPath = PATH_MANAGER->FindPath(strPathKey);
    wstring strPath;
    if (pPath)
        strPath = pPath;

    strPath += pBaseFolderName;
    assert(strPath.back() == L'\\' || strPath.back() == L'/');

    const auto extract_key = [](const char* str, int size)
    {
        int ed = size - 1;
        while (str[ed] != L'.') --ed;
        int st = ed - 1;
        while (str[st] != L'\\' && str[st] != L'/') st--;
        return string(str + st + 1, str + ed);
    };

    for (const auto& entry : fs::directory_iterator(strPath))
    {
        const wchar_t* path = entry.path().c_str();
        string strkey = extract_key(GetChar(path), lstrlen(path));
        Texture* pTex = RESOURCE_MANAGER->LoadTexture(strkey, path, "");
        m_optTexKey.emplace_back(std::move(strkey));
        pTex->SetColorKey(255, 255, 255);
        SAFE_RELEASE(pTex);
    }
}

void MapEditScene::SetSelectTexture(Texture* tex)
{
    SAFE_RELEASE(m_pSelTexture);
    m_pSelTexture = tex;
}

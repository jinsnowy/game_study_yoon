#include "MapEditScene.h"
#include "StartScene.h"
#include "SceneManager.h"
#include "Layer.h"
#include "../Application/Window.h"
#include "../Object/StaticObj/Stage.h"
#include "../Collider/ColliderRect.h"
#include "../Core/Camera.h"
#include "../Core/Input.h"
#include "../Resources/PrototypeManager.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/Texture.h"
#include "../Resource.h"
#include "../Core/PathManager.h"
#include "../Sound/SoundManager.h"
#include "../Object/StaticObj/UIButton.h"
#include "../Object/StaticObj/UIPanel.h"
#include "../Object/StaticObj/UITileSelect.h"
#include "../Object/StaticObj/TileObject.h"
#include "../Object/MoveObj/MovableObject.h"
#include "../Object/Object.h"
#include "../Math.h"

wchar_t MapEditScene::m_strText1[MAX_PATH] = {};
wchar_t MapEditScene::m_strText2[MAX_PATH] = {};

MapEditScene::MapEditScene()
{
    m_vecStage.resize(ST_END, nullptr);
    m_eSel = TEM_GROUND;
}

MapEditScene::~MapEditScene()
{
    INPUT->DeleteKey("ChangeOption");
    INPUT->DeleteKey("ChangeStage");
    INPUT->DeleteKey("ResetStage");
    INPUT->DeleteKey("Save");
    INPUT->DeleteKey("Load");

    DeleteAllEditObjects();

    StageClear(ST_GROUND, "Ground");
    StageClear(ST_STATIC, "Static");
    
    SAFE_RELEASE(m_pSelUI);
    SAFE_RELEASE(m_pSelTexture);
    SAFE_RELEASE(m_pSelObject);
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

    INPUT->AddKey("ChangeStage", VK_TAB);
    INPUT->AddKey("ResetStage", VK_CONTROL, 'R');
    INPUT->AddKey("Save", VK_CONTROL, 'S');
    INPUT->AddKey("Load", VK_CONTROL, 'O');

    return true;
}

void MapEditScene::Input(float dt)
{
    Scene::Input(dt);
    CameraScroll(dt);

    m_pSelUI->Input(dt);

    if (KEYPRESS("MouseLButton"))
    {
        Pos tMouseClientPos = MOUSECLIENTPOS;
        Pos tMouseWorldPos = MOUSEWORLDPOS;

        switch (m_eSel)
        {
            case TEM_GROUND:
            case TEM_STATIC:
            case TEM_OPTION:
            {
                Texture* pTex = m_pSelUI->SelectTile(tMouseClientPos);
                if (pTex) SetSelectTexture(pTex);
                else if (!m_pSelUI->SelectUITag(tMouseClientPos))
                {
                    switch (m_eSel)
                    {
                    case TEM_GROUND:
                    case TEM_STATIC:
                        m_vecStage[m_eCurStage]->ChangeTileTexture(tMouseWorldPos, m_pSelTexture);
                        break;
                    case TEM_OPTION:
                        m_vecStage[ST_GROUND]->ChangeTileOption(tMouseWorldPos, GetCurOption());
                        break;
                    }
                }
            }break;
            case TEM_OBJECT:
            {
                Object* pObj = m_pSelUI->SelectObject(tMouseClientPos);
                if (pObj) SetSelectObject(pObj);
                else if (!m_pSelUI->SelectUITag(tMouseClientPos))
                {
                    AddObject(CloneObject(m_pSelObject, tMouseWorldPos));
                }
            }
        }
    }

    if (KEYPRESS("MouseRButton"))
    {
        Pos tMouseWorldPos = MOUSEWORLDPOS;
        SAFE_RELEASE(m_pSelTexture);
        SAFE_RELEASE(m_pSelObject);
        switch (m_eSel)
        {
        case TEM_GROUND:
        case TEM_STATIC:
            m_vecStage[m_eCurStage]->SetTileNone(tMouseWorldPos);
            break;
        case TEM_OPTION:
            m_vecStage[ST_GROUND]->ChangeTileOption(tMouseWorldPos, TO_NONE);
            break;
        case TEM_OBJECT:
            if (m_pSelUI->IsObjectTileMode())
            {
                DeleteTileObject(tMouseWorldPos);
            }
            else {
                DeleteTileFreeObject(tMouseWorldPos);
            }
        }
    }

    if (KEYDOWN("ChangeStage"))
    {
        ChangeStage();
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
    DrawRedRect(hDC, MakeRect(left, top, TILESIZE, TILESIZE));

    switch (m_eSel)
    {
        case TEM_GROUND:
        case TEM_STATIC:
        case TEM_OPTION:
        {
            if (m_pSelTexture)
            {
                m_pSelTexture->DrawImageAtFixedSize(hDC, MOUSECLIENTPOS, TILESIZE);
            }
        }break;
        case TEM_OBJECT:
        {
            if (m_pSelObject)
            {
                m_pSelObject->DrawImageAt(hDC, MOUSECLIENTPOS);
            }
        }break;
    }

#ifdef _DEBUG
    stringstream ss;
    for (int i = 0; i < ST_END; ++i)
    {
        const string& tileOption = ConvertToNameOption(m_vecStage[i]->GetTileOption(MOUSEWORLDPOS));
        const string& tileName = m_vecStage[i]->GetTileName(MOUSEWORLDPOS);
        const string& stageName = m_vecStage[i]->GetTag();
        const string& objName = GetNearObjectName(MOUSEWORLDPOS);
        ss << "[" << stageName << "]: " << tileName << ", " << tileOption << " Object: " << objName << "\n";
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
    m_iTileNumX = numX;
    m_iTileNumY = numY;
    SetUpBaseStage(ST_GROUND, "Ground", numX, numY);
    SetUpBaseStage(ST_STATIC, "Static", numX, numY);
}

void MapEditScene::SaveDefaultStages(const char* fileName)
{
    FILE * pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "wb");

    m_vecStage[ST_GROUND]->SaveFromFile(pFile);
    m_vecStage[ST_STATIC]->SaveFromFile(pFile);

    int objNum = m_objList.size();
    if (objNum > 0)
    {
        fwrite(&objNum, 4, 1, pFile);

        list<Object*>::const_iterator iter = m_objList.begin();
        list<Object*>::const_iterator iterEnd = m_objList.end();
        for (; iter != iterEnd; ++iter)
        {
            bool hasPrototype = (*iter)->HasPrototype();
            if (hasPrototype)
            {
                string prototypeTag = (*iter)->GetPrototypeTag();
                int length = prototypeTag.size();
                fwrite(&hasPrototype, 1, 1, pFile);
                fwrite(&length, 4, 1, pFile);
                fwrite(prototypeTag.c_str(), 1, length, pFile);
            }
            (*iter)->SaveFromFile(pFile);
        }
    }

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
    DeleteAllEditObjects();

    FILE* pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "rb");

    LoadStage(ST_GROUND, "Ground", pFile);
    LoadStage(ST_STATIC, "Static", pFile);

    m_iTileNumX = m_vecStage[ST_GROUND]->GetTileSize().x;
    m_iTileNumY = m_vecStage[ST_GROUND]->GetTileSize().y;

    int objNum;
    fread(&objNum, 4, 1, pFile);
    if (objNum > 0)
    {
        int length = 0;
        char strTag[MAX_PATH] = { 0 };
        bool hasPrototype = false;
        Object* pObj = nullptr;
        for (int i = 0; i < objNum; ++i)
        {
            fread(&hasPrototype, 1, 1, pFile);
            if (hasPrototype)
            {
                fread(&length, 4, 1, pFile);
                fread(strTag, 1, length, pFile);
                strTag[length] = 0;
                string prototypeKey = string(strTag);
                pObj = PROTOTYPE_MANAGER->FindPrototype(prototypeKey)->Clone();
                pObj->Load(pFile);
                AddObject(pObj);
            }
        }
    }

    if (pFile)
    {
        fclose(pFile);
    }
}

void MapEditScene::LoadStage(STAGE_TAG eStageTag, const string& strlayerTag, FILE* pFile)
{
    StageClear(eStageTag, strlayerTag);
    Layer* pStageLayer = FindLayer(strlayerTag);
    m_vecStage[eStageTag] = Object::CreateObject<Stage>(strlayerTag, pStageLayer);
    m_vecStage[eStageTag]->LoadFromFile(pFile);
}

TILE_OPTION MapEditScene::GetCurOption() const
{
    if (m_pSelTexture)
    {
        const string& strTag = m_pSelTexture->GetTag();
        if (strTag == "1.NoOption")
            return TO_NONE;
        if (strTag == "2.NoMove")
            return TO_NOMOVE;
        if (strTag == "3.CropGround")
            return TO_CROP_GROUND;
    }
    return TO_NONE;
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
    m_pSelUI->LoadTiles(SEL_STATIC, L"SV/TileStatic/");
    m_pSelUI->LoadTiles(SEL_OPTION, L"SV/Option/");
    m_pSelUI->LoadTiles(SEL_NUMBER, L"SV/Numbers/Select/");

    m_pSelUI->LoadPrototypes(OBJ_TILE_INNER);

    m_pSelUI->InitUI();
}

void MapEditScene::BackButtonCallback(float dt)
{
    SOUND_MANAGER->PlaySound("StartScene_Click");
    SCENE_MANAGER->CreateScene<StartScene>(SC_NEXT);
}

void MapEditScene::ChangeStage()
{
    m_btn[m_eSel]->SetImageOffset(0, 0);
    m_eSel = static_cast<TILE_EDIT_MODE>((m_eSel + 1) % int(TEM_END));
    m_btn[m_eSel]->SetImageOffset(120, 0);

    if (m_eSel == 0)
    {
        m_eCurStage = ST_GROUND;
    }
    else if(m_eSel == 1)
    {
        m_eCurStage = ST_STATIC;
    }
 
    switch (m_eSel)
    {
    case TEM_GROUND:
        m_pSelUI->SetCurSelect(SEL_GROUND);
        break;
    case TEM_STATIC:
        m_pSelUI->SetCurSelect(SEL_STATIC);
        break;
    case TEM_OPTION:
        m_pSelUI->SetCurSelect(SEL_OPTION);
        break;
    case TEM_OBJECT:
        m_pSelUI->SetCurSelect(SEL_OBJECT);
        break;
    }
}

void MapEditScene::SetSelectTexture(Texture* tex)
{
    SAFE_RELEASE(m_pSelTexture);
    m_pSelTexture = tex;
}

string MapEditScene::ConvertToNameOption(TILE_OPTION eOpt) const
{
    switch (eOpt)
    {
    case TO_NONE:
        return "NoOption";
    case TO_NOMOVE:
        return "NoMove";
    case TO_CROP_GROUND:
        return "CropGround";
    default:
        return "Invalid";
    }
}

string MapEditScene::GetNearObjectName(const Pos& worldPos)
{
    float minDist = FLT_MAX;
    Object* pObj = nullptr;
    auto iterEnd = m_objList.end();
    for (auto iter = m_objList.begin(); iter != iterEnd; ++iter)
    {
        float dist = Math::Distance( (*iter)->GetPos(), worldPos);
        if (dist < TILESIZE && dist < minDist)
        {
            pObj = (*iter);
            minDist = dist;
        }
    }
    if (pObj)
    {
        return pObj->GetTag();
    }
    return "";
}

void MapEditScene::SetSelectObject(Object* pObj)
{
    SAFE_RELEASE(m_pSelTexture);
    SAFE_RELEASE(m_pSelObject);
    m_pSelObject = pObj;
}

void MapEditScene::CameraScroll(float dt)
{
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

void MapEditScene::AddObject(Object* pClone)
{
    if (pClone == nullptr) return;
    Object::AddObject(pClone);
    Layer* objLayer = Scene::FindLayer("Object");
    objLayer->AddObject(pClone);
    m_objList.push_back(pClone);
}

Object* MapEditScene::CloneObject(Object* pObj, const Pos& worldPos)
{
    if (!pObj) return nullptr;
    int clickIndex = m_vecStage[ST_GROUND]->GetTileIndex(worldPos);
    if (clickIndex == -1) return nullptr;

    TileObject* tClone = dynamic_cast<TileObject*>(pObj->Clone());
    if (tClone)
    {
        tClone->SetTileIndex(clickIndex);
        int row = clickIndex / m_iTileNumY + 1;
        int col = clickIndex % m_iTileNumY;
        tClone->SetPos(col * TILESIZE, row * TILESIZE);
        return tClone;
    }
    return pObj->Clone();
}

void MapEditScene::DeleteTileObject(const Pos& worldPos)
{
    int targetIndex = m_vecStage[ST_GROUND]->GetTileIndex(worldPos);
    if (targetIndex == -1) return;
    int index = -1;
    auto iterEnd = m_objList.end();
    for (auto iter = m_objList.begin(); iter != iterEnd; ++iter)
    {
        TileObject* pTileObj = dynamic_cast<TileObject*>(*iter);
        if (pTileObj)
        {
            index = pTileObj->GetTileIndex();
        }
        if (index == targetIndex)
        {
            Object::EraseObject((*iter));
            FindLayer("Object")->EraseObject((*iter));
            SAFE_RELEASE((*iter));
            m_objList.erase(iter);
            return;
        }
    }
}

void MapEditScene::DeleteTileFreeObject(const Pos& worldPos)
{
    float minDist = FLT_MAX;
    Object* pObj = nullptr;
    list<Object*>::iterator iterTarget;
    auto iterEnd = m_objList.end();
    for (auto iter = m_objList.begin(); iter != iterEnd; ++iter)
    {
        MovableObject* pMoveObj = dynamic_cast<MovableObject*>(*iter);
        if (pMoveObj)
        {
            float dist = Math::Distance(pMoveObj->GetPos(), worldPos);
            if (dist < 100.f && dist < minDist)
            {
                pObj = pMoveObj;
                minDist = dist;
                iterTarget = iter;
            }
        }
    }
    if (pObj)
    {
        Object::EraseObject(pObj);
        FindLayer("Object")->EraseObject(pObj);
        SAFE_RELEASE(pObj);
        m_objList.erase(iterTarget);
        return;
    }
}

void MapEditScene::DeleteAllEditObjects()
{
    list<Object*>::iterator iter;
    list<Object*>::iterator iterEnd = m_objList.end();

    Layer* pLayer = FindLayer("Object");
    for (iter = m_objList.begin(); iter != iterEnd; ++iter)
    {
        Object::EraseObject((*iter));
        FindLayer("Object")->EraseObject((*iter));
        SAFE_RELEASE((*iter));
    }
    m_objList.clear();
}

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
#include "../Object/MoveObj/MovableObject.h"
#include "../Object/Object.h"
#include "../Object/StaticObj/Tile.h"
#include "../Object/StaticObj/Tree.h"
#include "../Math.h"

wchar_t MapEditScene::m_strText1[MAX_PATH] = {};
wchar_t MapEditScene::m_strText2[MAX_PATH] = {};

MapEditScene::MapEditScene()
{
    m_vecStage.resize(ST_END, nullptr);
}

MapEditScene::~MapEditScene()
{
    INPUT->DeleteKey("ChangeOption");
    INPUT->DeleteKey("ChangeState");
    INPUT->DeleteKey("ResetStage");
    INPUT->DeleteKey("Save");
    INPUT->DeleteKey("Load");

    EmptyMapEditScene();

    SAFE_RELEASE(m_pSelUI);
    SAFE_RELEASE(m_pSelObject);
}

bool MapEditScene::Init()
{
    if (!Scene::Init())
    {
        return false;
    }


    SetUpDefaultStages(50, 50);

    SetUpBackButton();

    SetUpTileSelectUI();

    SetUpCamera();

    INPUT->AddKey("ChangeState", VK_TAB);
    INPUT->AddKey("ResetStage", VK_CONTROL, 'R');
    INPUT->AddKey("Save", VK_CONTROL, 'S');
    INPUT->AddKey("Load", VK_CONTROL, 'O');

    m_pObjLayer = FindLayer("Object");

    return true;
}

void MapEditScene::Input(float dt)
{
    Scene::Input(dt);
    CameraScroll(dt);

    m_pSelUI->Input(dt);
    if (KEYDOWN("ChangeState"))
    {
        m_pSelUI->ChangeState();
        SAFE_RELEASE(m_pSelObject);
        switch (m_pSelUI->GetCurSelect())
        {
        case SEL_GROUND:
        case SEL_OPTION:
            m_eCurStage = ST_GROUND;
            break;
        case SEL_STATIC:
            m_eCurStage = ST_STATIC;
            break;
        }
    }

    if (KEYPRESS("MouseLButton"))
    {
        Pos tMouseClientPos = MOUSECLIENTPOS;
        Pos tMouseWorldPos = MOUSEWORLDPOS;

        bool touchUI = false;
        Object* pObj = m_pSelUI->GetClickObject(tMouseClientPos, touchUI);
        if (pObj)
        {
            SetSelectObject(pObj);
        }
        if (!touchUI && m_pSelObject)
        {
            Object* pClone = EditCloneObject(m_pSelObject, tMouseWorldPos);
            const string& texTag = m_pSelObject->AccessTexture()->GetTag();
            switch (m_pSelUI->GetCurSelect())
            {
            case SEL_GROUND:
            case SEL_STATIC:
                m_vecStage[m_eCurStage]->ChangeTileByCloneTile(tMouseWorldPos, static_cast<Tile*>(pClone));
                break;
            case SEL_OPTION:
                m_vecStage[ST_STATIC]->ChangeTileOption(tMouseWorldPos, m_pSelUI->GetOpt(texTag));
                break;
            default:
                m_pObjLayer->AddObject(pClone);
                break;
            }
            SAFE_RELEASE(pClone);
        }
    }

    if (KEYPRESS("MouseRButton"))
    {
        Pos tMouseWorldPos = MOUSEWORLDPOS;
        if (m_pSelObject)
        {
            SAFE_RELEASE(m_pSelObject);
        }
        else {
            switch (m_pSelUI->GetCurSelect())
            {
            case SEL_GROUND:
            case SEL_STATIC:
                m_vecStage[m_eCurStage]->SetTileNone(tMouseWorldPos);
                break;
            case SEL_OPTION:
                m_vecStage[ST_STATIC]->ChangeTileOption(tMouseWorldPos, TO_NONE);
                break;
            default:
                DeleteNearObject(tMouseWorldPos);
                break;
            }
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
                SetUpCamera();
            }
        }
    }
    if (KEYDOWN("Save"))
    {
        ShowCursor(TRUE);
        INT_PTR res = DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1), WINDOWHANDLE, MapEditScene::DlgProc1);
        ShowCursor(FALSE);

        if (res == IDOK)
        {
            char strFileName[MAX_PATH] = {};
            WideCharToMultiByte(CP_ACP, 0, m_strText1, -1, strFileName, lstrlen(m_strText1), 0, 0);
            SaveDefaultStages(strFileName);
        }
    }
    if (KEYDOWN("Load"))
    {
        ShowCursor(TRUE);
        INT_PTR res = DialogBox(WINDOWINSTANCE, MAKEINTRESOURCE(IDD_DIALOG1), WINDOWHANDLE, MapEditScene::DlgProc1);
        ShowCursor(FALSE);

        if (res == IDOK)
        {
            char strFileName[MAX_PATH] = {};
            WideCharToMultiByte(CP_ACP, 0, m_strText1, -1, strFileName, lstrlen(m_strText1), 0, 0);
            LoadDefaultStages(strFileName);
            SetUpCamera();
        }
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
            // Edit Box���� ���ڿ��� ���´�.
            memset(m_strText1, 0, sizeof(wchar_t) * MAX_PATH);
            GetDlgItemText(hWnd, IDC_EDIT1, m_strText1, MAX_PATH);
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

    // ���콺 ���� Ÿ�� ��ο�
    Pos tPos = MOUSEWORLDPOS;
    int index = m_vecStage[ST_GROUND]->GetTileIndex(tPos);
    Pos tilePos = m_vecStage[ST_GROUND]->GetTilePos(index);
    tilePos -= CAMERA->GetTopLeft();
    DrawRedRect(hDC, MakeRect(int(tilePos.x), int(tilePos.y), TILESIZE, TILESIZE));

    if (m_pSelObject)
    {
        m_pSelObject->DrawImageAt(hDC, MOUSECLIENTPOS);
    }

#ifdef _DEBUG
    stringstream ss;
    for (int i = 0; i < ST_END; ++i)
    {
        const string& tileOption = ConvertToNameOption(m_vecStage[i]->GetTileOption(MOUSEWORLDPOS));
        const string& tileName = m_vecStage[i]->GetTileName(MOUSEWORLDPOS);
        const string& stageName = m_vecStage[i]->GetTag();
        const string& objName = GetNearObjectName(MOUSEWORLDPOS);
        ss << "[" << stageName << "]: " << tileName << ", " << tileOption;
        if (objName.size())
        {
            ss << " Object: " << objName;
        }
        ss << "\n";
        size_t length = ss.str().size();
        TextOut(hDC, MOUSECLIENTPOS.x, MOUSECLIENTPOS.y - 100 + 20 * i, GetWChar(ss.str().c_str()), length);
        ss.clear();
        ss.str("");
    }
#endif
}

void MapEditScene::SetUpCamera()
{
    CAMERA->SetWorldResolution(m_iTileNumX * TILESIZE, m_iTileNumY * TILESIZE);
    CAMERA->SetPos(0.f, 0.f);
    CAMERA->SetPivot(0.f, 0.f);
    CAMERA->ReleaseTarget();
}

void MapEditScene::SetUpDefaultStages(int numX, int numY)
{
    EmptyMapEditScene();

    m_iTileNumX = numX;
    m_iTileNumY = numY;

    SetUpBaseStage(ST_GROUND, "GroundStage", "Ground", numX, numY);
    SetUpBaseStage(ST_STATIC, "StaticStage", "Static", numX, numY);
}

void MapEditScene::SaveDefaultStages(const char* fileName)
{
    FILE * pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "wb");

    m_vecStage[ST_GROUND]->SaveFromFile(pFile);
    m_vecStage[ST_STATIC]->SaveFromFile(pFile);

    const auto& objList = m_pObjLayer->GetObjList();
    size_t objNum = objList->size();
    if (objNum > 0)
    {
        fwrite(&objNum, sizeof(objNum), 1, pFile);

        list<Object*>::const_iterator iter = objList->begin();
        list<Object*>::const_iterator iterEnd = objList->end();
        for (; iter != iterEnd; ++iter)
        {
            int eType = (int) (*iter)->GetObjectType();

            fwrite(&eType, 4, 1, pFile);
            bool hasPrototype = (*iter)->HasPrototype();
            fwrite(&hasPrototype, 1, 1, pFile);
            if (hasPrototype)
            {
                string prototypeTag = (*iter)->GetPrototypeTag();
                size_t length = prototypeTag.size();
             
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

void MapEditScene::SetUpBaseStage(STAGE_TAG eStageTag, const string& objTag, const string& strlayerTag, int numX, int numY)
{
    StageClear(eStageTag, strlayerTag);
    Layer* pStageLayer = FindLayer(strlayerTag);
    m_vecStage[eStageTag] = CreateObject<Stage>(objTag, pStageLayer);
    m_vecStage[eStageTag]->CreateTile(numX, numY);
}

void MapEditScene::LoadDefaultStages(const char* fileName)
{
    EmptyMapEditScene();

    FILE* pFile = PATH_MANAGER->FileOpen(fileName, DATA_PATH, "rb");
    if (pFile == NULL)
    {
        throw EXCEPT(L"File Not Exists");
    }

    LoadStage(ST_GROUND, "GroundStage", "Ground", pFile);
    LoadStage(ST_STATIC, "StaticStage", "Static", pFile);

    m_iTileNumX = m_vecStage[ST_GROUND]->GetStageTileNumX();
    m_iTileNumY = m_vecStage[ST_GROUND]->GetStageTileNumY();

    size_t objNum;
    fread(&objNum, sizeof(objNum), 1, pFile);
    if (objNum > 0)
    {
        int length = 0;
        char strTag[MAX_PATH] = { 0 };
        int objType = 0;
        bool hasPrototype = false;
        Object* pObj = nullptr;
        for (int i = 0; i < objNum; ++i)
        {
            fread(&objType, 4, 1, pFile);
            fread(&hasPrototype, 1, 1, pFile);
            if (hasPrototype)
            {
                fread(&length, 4, 1, pFile);
                fread(strTag, 1, length, pFile);
                strTag[length] = 0;
                string prototypeKey = string(strTag);
                pObj = PROTOTYPE_MANAGER->FindPrototype(prototypeKey)->Clone();
            }
            else {
                switch (objType)
                {
                case OBJ_TREE:
                    pObj = Object::CreateObject<Tree>("Tree");
                    break;
                case OBJ_TILE:
                    pObj = Object::CreateObject<Tile>("None");
                    break;
                }
            }
            pObj->Load(pFile);
            m_pObjLayer->AddObject(pObj);
            SAFE_RELEASE(pObj);
        }
    }

    if (pFile)
    {
        fclose(pFile);
    }
}

void MapEditScene::LoadStage(STAGE_TAG eStageTag, const string& objTag, const string& strlayerTag, FILE* pFile)
{
    StageClear(eStageTag, strlayerTag);
    Layer* pStageLayer = FindLayer(strlayerTag);
    m_vecStage[eStageTag] = CreateObject<Stage>(objTag, pStageLayer);
    m_vecStage[eStageTag]->LoadFromFile(pFile);
}

TILE_OPTION MapEditScene::GetCurOption() const
{
    if (m_pSelObject)
    {
        return m_pSelUI->GetOpt(m_pSelObject->AccessTexture()->GetTag());
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
        SAFE_RELEASE(m_vecStage[eStageTag]);
    }
}

void MapEditScene::SetUpBackButton()
{
    Layer* pLayer = FindLayer("UI");
    UIButton* pBackBtn = CreateObject<UIButton>("BackButton", pLayer);
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
}

void MapEditScene::SetUpTileSelectUI()
{
    Layer* pLayer = Scene::FindLayer("UI");
    m_pSelUI = CreateObject<UITileSelect>("SelectUI", pLayer);

    m_pSelUI->LoadTiles(SEL_GROUND, L"SV/TileGround/");
    m_pSelUI->LoadTiles(SEL_STATIC, L"SV/TileStatic/");
    m_pSelUI->LoadTiles(SEL_OPTION, L"SV/Option/");
    m_pSelUI->LoadTiles(SEL_TILEOBJECT, L"SV/TileObject/Background/");
    m_pSelUI->LoadTiles(SEL_TILEOBJECT, L"SV/TileObject/Building/");
    m_pSelUI->LoadTiles(SEL_TILEOBJECT, L"SV/TileObject/Inner/");
    m_pSelUI->LoadTiles(SEL_TILEOBJECT, L"SV/TileObject/Outdoor/");
    m_pSelUI->LoadTiles(SEL_TILEOBJECT, L"SV/TileObject/Plant/");
    m_pSelUI->LoadTiles(SEL_TILEOBJECT, L"SV/TileObject/Wall/");

    // object prototype
    m_pSelUI->LoadPrototypes(PR_PLANT);

    m_pSelUI->SetUpTagButton(this);
  
}

void MapEditScene::BackButtonCallback(float dt)
{
    SceneState state;
    state.nextBeacon = BC_NONE;
    state.nextDir = RIGHT;
    state.nextScene = SC_START;
    SOUND_MANAGER->PlaySound("StartScene_Click");
    SCENE_MANAGER->SignalizeSceneChange(state);
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
    case TO_BEACON_1:
        return "Beacon1";
    case TO_BEACON_2:
        return "Beacond";
    case TO_BEACON_3:
        return "Beacon3";
    default:
        return "Invalid";
    }
}

string MapEditScene::GetNearObjectName(const Pos &worldPos)
{
  
    float minDist = FLT_MAX;
    Object* pObj = nullptr;
    Pos tPos = worldPos - Pos(TILESIZE / 2, -TILESIZE/2);

    const auto& objList = m_pObjLayer->GetObjList();
    auto iterEnd = objList->end();
    for (auto iter = objList->begin(); iter != iterEnd; ++iter)
    {
        float dist = Math::Distance( (*iter)->GetPos(), tPos);
        if (dist < TILESIZE/2 && dist < minDist)
        {
            pObj = (*iter);
            minDist = dist;
        }
    }
    if (pObj)
    {
        char buffer[30] = { 0 };
        sprintf_s(buffer, "Pos (%.1f,%.1f)", pObj->GetPos().x, pObj->GetPos().y);
        return pObj->GetPrototypeTag() + string(buffer);
    }
    return "";
}

void MapEditScene::SetSelectObject(Object* pObj)
{
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

Object* MapEditScene::EditCloneObject(Object* const pObj, const Pos& worldPos)
{
    int clickIndex = m_vecStage[ST_GROUND]->GetTileIndex(worldPos);
    if (clickIndex == -1) return nullptr;

    Object* pClone = pObj->Clone();

    Tile* tileClone = dynamic_cast<Tile*>(pClone);
    int row = clickIndex / m_iTileNumX + 1;
    int col = clickIndex % m_iTileNumX;
    Pos offset(col * TILESIZE, row * TILESIZE);

    if (tileClone)
    {
        tileClone->SetPos(offset);
        return tileClone;
    }

    pClone->AddOffset(offset);
    return pClone;
}

void MapEditScene::DeleteNearObject(const Pos& worldPos)
{
    float minDist = FLT_MAX;
    Object* pObj = nullptr;

    const auto& objList = m_pObjLayer->GetObjList();
    auto iterEnd = objList->end();

    for (auto iter = objList->begin(); iter != iterEnd; ++iter)
    {
        if ((*iter))
        {
            float dist = Math::Distance((*iter)->GetPos(), worldPos);
            if (dist < TILESIZE && dist < minDist)
            {
                pObj = (*iter);
                minDist = dist;
            }
        }
    }

    if (pObj)
    {
        m_pObjLayer->EraseObject(pObj);
        SAFE_RELEASE(pObj);
        return;
    }
}

void MapEditScene::EmptyMapEditScene()
{
    StageClear(ST_GROUND, "Ground");
    StageClear(ST_STATIC, "Static");

    if(m_pObjLayer)
        m_pObjLayer->ClearObject();
}

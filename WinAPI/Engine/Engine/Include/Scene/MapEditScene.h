#pragma once
#include "Scene.h"

class MapEditScene :
    public Scene
{
private:
    friend class SceneManager;
private:
    MapEditScene();
    ~MapEditScene();
public:
    virtual bool Init();
    virtual void Input(float dt);
private:
    int m_iTileNumX, m_iTileNumY;
    static wchar_t m_strText1[MAX_PATH];
    static wchar_t m_strText2[MAX_PATH];

    class Object* m_pSelObject = nullptr;
    class UITileSelect* m_pSelUI = nullptr;

    STAGE_TAG m_eCurStage = ST_GROUND;
private:
    static INT_PTR CALLBACK DlgProc1(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK DlgProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void BackButtonCallback(float dt);
    string GetNearObjectName(const Pos& worldPos);
    string ConvertToNameOption(TILE_OPTION eOpt) const;
    TILE_OPTION GetCurOption() const;
    void SetSelectObject(class Object* pObj);
    void SetSelectTexture(class Texture* pTex);
public:
    virtual void Draw(HDC hDC, float dt);
private:
    void SetUpCamera();
    void SetUpBackButton();
    void SetUpTileSelectUI();
    void SetUpBaseStage(STAGE_TAG eStageTag, const string& objTag, const string& strlayerTag, int numX, int numY);
    void SetUpDefaultStages(int numX, int numY);
    void SaveDefaultStages(const char* fileName);
    void LoadStage(STAGE_TAG eStageTag, const string& objTag, const string& strlayerTag, FILE* pFile);
    void LoadDefaultStages(const char* fileName);
    void StageClear(STAGE_TAG eStageTag, const string& layerTag);
    void CameraScroll(float dt);
private:
    vector<class Stage*> m_vecStage;
    // 게임상에 object layer에 추가될 오브젝트 입니다.
    list<class Object*> m_objList;
public:
    Object* CreateCloneObject(Object* const pObj, const Pos& worldPos);

    void AddObject(Object* pClone);
    void DeleteObject(Object* pObj);
    void DeleteNearObject(const Pos& worldPos);
    void EmptyMapEditScene();
};
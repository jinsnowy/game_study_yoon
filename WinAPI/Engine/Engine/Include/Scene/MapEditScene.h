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
    TILE_EDIT_MODE  m_eTem;
    TILE_OPTION     m_eEditOption;

    // vector<class Texture*> m_vecTileTex;
    Texture* m_pSelTexture = nullptr;
    class UITileSelect* m_pSelUI = nullptr;
    STAGE_TAG m_eCurStage = ST_GROUND;

    int             m_iEditTileTex;
    vector<class Stage*>    m_vecStage;
    static wchar_t m_strText[MAX_PATH];
private:
    static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void BackButtonCallback(float dt);
public:
    virtual void Draw(HDC hDC, float dt);
private:
    void SetUpCamera();
    void SetUpBackButton();
    void SetUpTileSelectUI();
    void SetUpBaseStage(STAGE_TAG eStageTag, const string& strlayerTag, int numX, int numY);
    void SetUpDefaultStages(int numX, int numY);
    void LoadStage(STAGE_TAG eStageTag, const string& strlayerTag, const char* fileName);
    void LoadDefaultStages(const char* fileName);
};
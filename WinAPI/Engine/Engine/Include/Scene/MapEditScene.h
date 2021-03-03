#pragma once
#include "Scene.h"

class MapEditScene :
    public Scene
{
private:
    friend class SceneManager;
    const vector<wstring> m_btnFileName = { L"GroundSelTag", L"ObjectSelTag", L"StaticTag"};
private:
    MapEditScene();
    ~MapEditScene();
public:
    virtual bool Init();
    virtual void Input(float dt);
private:
    TILE_EDIT_MODE  m_eTem;
    TILE_OPTION     m_eEditOption;

    class Texture* m_pSelTexture = nullptr;
    class UITileSelect* m_pSelUI = nullptr;
    class UIButton* m_pSelButton = nullptr;
    STAGE_TAG m_eCurStage = ST_GROUND;

    vector<class Stage*> m_vecStage;
    vector<string> m_optTexKey;
    static wchar_t m_strText[MAX_PATH];
private:
    static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void BackButtonCallback(float dt);
    void StageButtonCallback(class UIButton* btn, int type, float dt);
    void LoadOptionTiles(const wchar_t* pBaseFolderName, const string& strPathKey = TEXTURE_PATH);
    void SetSelectTexture(class Texture* tex);
public:
    virtual void Draw(HDC hDC, float dt);
private:
    void SetUpCamera();
    void SetUpUIButton();
    void SetUpTileSelectUI();
    void SetUpBaseStage(STAGE_TAG eStageTag, const string& strlayerTag, int numX, int numY);
    void SetUpDefaultStages(int numX, int numY);
    void SaveDefaultStages(const char* fileName);
    void LoadStage(STAGE_TAG eStageTag, const string& strlayerTag, FILE* pFile);
    void LoadDefaultStages(const char* fileName);
    void StageClear(STAGE_TAG eStageTag, const string& layerTag);
};
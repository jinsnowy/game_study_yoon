#pragma once
#include "Scene.h"

class MapEditScene :
    public Scene
{
private:
    friend class SceneManager;
    const vector<wstring> m_btnFileName = { L"1.GroundSelTag",
                                            L"2.StaticSelTag",
                                            L"3.ObjectSelTag",
                                            L"4.ObjectSelTag2",
                                            L"5.OptionSelTag"};
    vector<class UIButton*> m_btn;
private:
    MapEditScene();
    ~MapEditScene();
public:
    virtual bool Init();
    virtual void Input(float dt);
private:
    int m_iTileNumX, m_iTileNumY;
    TILE_EDIT_MODE  m_eSel;
    static wchar_t m_strText1[MAX_PATH];
    static wchar_t m_strText2[MAX_PATH];

    class Texture* m_pSelTexture = nullptr;
    class UITileSelect* m_pSelUI = nullptr;

    STAGE_TAG m_eCurStage = ST_GROUND;
private:
    static INT_PTR CALLBACK DlgProc1(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static INT_PTR CALLBACK DlgProc2(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void BackButtonCallback(float dt);
    //void LoadOptionTiles(const wchar_t* pBaseFolderName, const string& strPathKey = TEXTURE_PATH);
    void SetSelectTexture(class Texture* tex);
public:
    virtual void Draw(HDC hDC, float dt);
private:
    void ChangeStage();
    void SetUpCamera();
    void SetUpUIButton();
    void SetUpTileSelectUI();
    void SetUpBaseStage(STAGE_TAG eStageTag, const string& strlayerTag, int numX, int numY);
    void SetUpDefaultStages(int numX, int numY);
    void SaveDefaultStages(const char* fileName);
    void LoadStage(STAGE_TAG eStageTag, const string& strlayerTag, FILE* pFile);
    void LoadDefaultStages(const char* fileName);
    void StageClear(STAGE_TAG eStageTag, const string& layerTag);
    void CameraScroll(float dt);
    TILE_OPTION GetCurOption() const;
private:
    vector<class Stage*> m_vecStage;
    // ���ӻ� object layer�� �߰��� ������Ʈ �Դϴ�.
    list<class Object*> m_objList;
public:
    void DeleteTileObject(int index);
    void DeleteTileFreeObject(const Pos& worldPos);
    void DeleteAllEditObjects();
};
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

    Texture* m_pSelTexture = nullptr;
    class UITileSelect* m_pSelUI = nullptr;

    vector<class Texture*> m_vecTileTex;
    int             m_iEditTileTex;
    class Stage*    m_pStage;
    static wchar_t m_strText[MAX_PATH];
public:
    static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void BackButtonCallback();
public:
    virtual void Draw(HDC hDC, float dt);
private:
    void SetUpCamera();
    void SetUpBaseTiles();
    void SetUpBackButton();
    void SetUpTileSelectUI();
};
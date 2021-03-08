#pragma once
#include "Scene.h"
class GameScene : public Scene
{
private:
    friend class SceneManager;
    class Stage* m_pGroundStage;
    class Stage* m_pObjStage;
    int m_iTileNumX;
    int m_iTileNumY;
protected:
    GameScene();
    ~GameScene() = 0;
public:
    int GetTileIndex(const Pos& worldPos);
    INDEX GetTileRowColIndex(const Pos& worldPos);
    INDEX IndexDiff(const Pos& pos, const Pos& from);
public:
    virtual void SetUpScene(const char* fileName, Object* pPlayer, Pos camPivot);
    virtual bool Init();
    virtual void Input(float dt);
    virtual void Update(float dt);
    virtual void LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hdc, float dt);
private:
    virtual void LoadStage(const string& objectTag, const string& strlayerTag, FILE* pFile) final;
    virtual void LoadDefaultStages(const char* fileName) final;
};
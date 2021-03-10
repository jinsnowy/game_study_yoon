#pragma once
#include "Scene.h"
class GameScene : public Scene
{
private:
    friend class SceneManager;
    class Stage* m_pGroundStage = nullptr;
    class Stage* m_pStaticStage = nullptr;
    class Player* m_pPlayer = nullptr;
    int m_iTileNumX = -1;
    int m_iTileNumY = -1;
protected:
    GameScene();
    ~GameScene() = 0;
public:
    bool IsBlockTile(const Pos& worldPos);
    int GetTileIndex(const Pos& worldPos);
    Pos GetTilePos(const Pos& worldPos);
    INDEX GetTileRowColIndex(const Pos& worldPos);
    INDEX IndexDiff(const Pos& pos, const Pos& from);
public:
    void SetUpScene(SceneState state, Player* player);
    virtual void SetUpScene(const char* fileName);
    virtual bool Init();
    virtual void Input(float dt);
    virtual void Update(float dt);
    virtual void LateUpdate(float dt);
    virtual void Collision(float dt);
    virtual void Draw(HDC hdc, float dt);
private:
    virtual void LoadStage(const string& objectTag, const string& strlayerTag, FILE* pFile) final;
    virtual void LoadDefaultStages(const char* fileName) final;
private:
    Pos FindBeacon(BEACON_TAG bc);
    void SetUpMainCharacter(Player* pPlayer);
    void SpawnMainCharacter(const Pos& worldPos);
};
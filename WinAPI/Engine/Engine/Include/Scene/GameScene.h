#pragma once
#include "Scene.h"
class GameScene : public Scene
{
private:
    friend class SceneManager;
protected:
    GameScene();
    ~GameScene() = 0;
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
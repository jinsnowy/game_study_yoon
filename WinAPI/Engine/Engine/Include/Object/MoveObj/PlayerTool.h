#pragma once
#include "../Object.h"
class PlayerTool :
	public Object

{
	friend class Object;
	friend class Player;
private:
	enum ToolState
	{
		TOOL_NONE, // Åø ¼±ÅÃ X
		TOOL_PICK, // °î±ªÀÌ
		TOOL_AXE,  // µµ³¢
		TOOL_HOE,  // È£¹Ì
		TOOL_WATER,// ¹°»Ñ¸®°³
		TOOL_SICKLE, // ³´
		TOOL_BLADE // ¹«±â
	};
	ToolState eToolState = TOOL_WATER;
	static constexpr float posOffsetX = -64.f;
	static constexpr float posOffsetY = 64.f;
	class Player* m_pPlayer = nullptr;
private:
	void SetPlayer(Player* player);
	void SetToolState(ToolState eState) { eToolState = eState; }
	void Play();
public:
	PlayerTool();
	PlayerTool(const PlayerTool& obj) = delete;
	~PlayerTool();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hDC, float dt);
	virtual void StateTransit(int iState);
	virtual PlayerTool* Clone() { throw EXCEPT(L"PlayerTool cloned"); return nullptr; }
private:
	void LoadWater();
	void LoadPick();
	void LoadHoe();
	void LoadAxe();
};
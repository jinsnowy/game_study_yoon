#pragma once
#include "UI.h"
#include "../../framework.h"
class UITileSelect :
    public UI
{
private:
	friend class Object;
	friend class Scene;
	int itemNumX = 0;
	int itemNumY = 0;
	int validWidth;
	int validHeight;
	int totalSizeX;
	int totalSizeY;
	friend class MapEditScene;
private:
	static constexpr int m_iSelButtonSize = 40;
	static constexpr int m_iStageTagWidth = 120;
	static constexpr int m_iStageTagHeight = 80;
	int m_iCurPageNum = 0;
	UISEL_TYPE m_eCurSel = SEL_GROUND;
	int m_iMarginWidth = 10;
	int m_iMarginHeight = 10;
	int m_iMarginItem = 10;
	vector<vector<Texture*>> m_BaseTiles;
public:
	void SetCurSelect(UISEL_TYPE eSel) 
	{
		m_eCurSel = eSel; m_iCurPageNum= 0;
	}
	Texture* SelectTile(const Pos& screenPos);
	bool SelectUITag(const Pos& screenPos);
	void SetMargin(int w, int h);
	void LoadTiles(UISEL_TYPE eSel, const wchar_t* pBaseFolderName, const string& strPathKey = TEXTURE_PATH);
protected:
	UITileSelect();
	UITileSelect(const UITileSelect& ui) = delete;
	virtual ~UITileSelect();
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
	virtual UITileSelect* Clone() { return nullptr; }
};


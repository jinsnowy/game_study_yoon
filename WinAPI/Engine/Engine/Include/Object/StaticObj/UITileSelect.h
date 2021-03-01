#pragma once
#include "UI.h"
#include "../../framework.h"
class UITileSelect :
    public UI
{
private:
	int itemNumX = 0;
	int itemNumY = 0;
	int validWidth;
	int validHeight;
	int totalSizeX;
	int totalSizeY;
	friend class MapEditScene;
	enum class UISEL_TYPE
	{
		SEL_GROUND = 0,
		SEL_NUMBER,
		SEL_END
	};
private:
	static constexpr int m_iSelButtonSize = 30;
	int m_iCurPageNum = 0;
	int m_iCurSel = 0;
	int m_iMarginWidth = 20;
	int m_iMarginHeight = 40;
	int m_iMarginItem = 20;
	friend class Object;
	friend class Scene;
	vector<vector<Texture*>> m_BaseTiles;
public:
	Texture* SelectTile(const Pos& screenPos);
	void SetMargin(int w, int h);
	void LoadNumbers(const wchar_t* pBaseFolderName = L"SV/Numbers/Select/", const string& strPathKey = TEXTURE_PATH);
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


#pragma once
#include "UI.h"
#include "../../framework.h"
class UITileSelect :
    public UI
{
private:
	friend class Object;
	friend class Scene;
	friend class MapEditScene;
	int m_iDrawMaxitemNumX;
	int m_iDrawMaxitemNumY;
	int m_iValidWidth;
	int m_iValidHeight;
	int m_iTotalSizeX;
	int m_iTotalSizeY;
private:
	int m_iCurPageNum = 0;
	UISEL_TYPE m_eCurSelTile = SEL_GROUND;
	OBJECT_TYPE m_eCurSelObject = OBJ_TILE_INNER;

	int m_iMarginWidth = 10;
	int m_iMarginHeight = 10;
	int m_iMarginItem = 10;
	static constexpr int m_iSelButtonSize = 40;
	static constexpr int m_iStageTagWidth = 120;
	static constexpr int m_iStageTagHeight = 80;
	vector<vector<Texture*>> m_BaseTiles;
	vector<unordered_map<string, Object*>*> m_PrototypeMapContainer;
public:
	bool IsObjectTileMode() const;
	void SetCurSelect(UISEL_TYPE eSel);
	Texture* SelectTile(const Pos& screenPos);
	Object* SelectObject(const Pos& screenPos);
	bool SelectUITag(const Pos& screenPos);
	void InitUI();
	void LoadTiles(UISEL_TYPE eSel, const wchar_t* pBaseFolderName, const string& strPathKey = TEXTURE_PATH);
	void LoadPrototypes(OBJECT_TYPE eType);
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
private:
	void DrawTilePanel(HDC hdc, float dt);
	void DrawObjectPanel(HDC hdc, float dt);
};


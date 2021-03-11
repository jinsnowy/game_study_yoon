#pragma once
#include "../../framework.h"
#include "UIPanel.h"
class UIGameManager
{
	DECLARE_SINGLE(UIGameManager)
private:
	class Clock {
	private:
		float m_fTimeCur = 0.f;
		static constexpr float m_fTimeUnit = 7.166f; // 게임 시간 10 분에 현실 시간 초 단위
	public:
		bool bAfterNoon = false;
		int hours = 6;
		int minutes = 0;
		int days = 1;
		int weekdays = 0;
	public:
		Clock() {};
		~Clock() {};
		void Tick(float dt);
		bool IsAfterNoon() { return bAfterNoon; }
		int GetWeekDays() const { return weekdays; }
		int GetDays() const { return days; }
		int GetHours()const { return hours; }
		int GetMinutes() const { return minutes; }
		void IncreaseTenMinutes() { Tick(m_fTimeUnit); }
	};
public:
	virtual bool Init();
	virtual void Input(float dt);
	virtual int Update(float dt);
	virtual int LateUpdate(float dt);
	virtual void Collision(float dt);
	virtual void Draw(HDC hdc, float dt);
private:
	static constexpr float m_fUpperMargin = 20.f;
	static constexpr int m_iNumberWidth = 18;
	static constexpr int m_iMaxDigits = 8;
	static constexpr int m_iMoneyDrawStartX = 64;
	static constexpr int m_iMoneyDrawStartY = 192;
	static constexpr int m_iMargin = 6;
	static constexpr int m_iMaxHours = 26;
	static constexpr float interval = float(m_iMaxHours - 6) / 7.f;

	static constexpr int m_iUpperDisplayStartY = 20;

	bool m_bTickStart = false;
	class Player* m_pPlayer = nullptr;
	class UIPanel* m_pTimeUI = nullptr;

	vector<class Texture*> m_vecNeedleTex;
	vector<class Texture*> m_vecMoneyTex;
	vector<class Texture*> m_vecWeekDays;
	vector<class Texture*> m_vecNoon;
	vector<class Texture*> m_vecNumbers;
	unique_ptr<Clock> m_clock = make_unique<Clock>();

	void DrawCurrentTime(HDC hdc);
	void DrawCurrentClockNeedle(HDC hdc);
	void DrawCurrentMoney(HDC hdc);
public:
	void StartTick() { m_bTickStart = true; }
	void IncreaseTime() { m_clock->IncreaseTenMinutes(); }
	void SetPlayer(Player* pPlayer);
};


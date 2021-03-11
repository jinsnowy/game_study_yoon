#include "UIGameManager.h"
#include "../../Resources/ResourceManager.h"
#include "../../Resources/Texture.h"
#include "../MoveObj/Player.h"
#include "../../Application/Window.h"
#include "../Object.h"
#include "../../Core/Input.h"

DEFINITION_SINGLE(UIGameManager);

bool UIGameManager::Init()
{
	m_pTimeUI = Object::CreateObject<UIPanel>("TimeUI");
	m_pTimeUI->SetTexture("ClockBase", L"SV/Scene/ClockBase.bmp");
	m_pTimeUI->SetColorKey(255, 255, 255);
	m_pTimeUI->SetAsTextureSize();
	m_pTimeUI->SetPos(GETRESOLUTION.x - m_pTimeUI->GetSize().x, m_fUpperMargin);

	COLORREF chromaKey = RGB(255, 255, 255);
	m_vecMoneyTex = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/Money/", chromaKey);
	m_vecNeedleTex = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/ClockNeedle/", chromaKey);
	m_vecWeekDays = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/WeekDays/", chromaKey);
	m_vecNoon = RESOURCE_MANAGER->LoadTextureFromDirectory(L"SV/Scene/Noon/", chromaKey);

	return true;
}

void UIGameManager::Input(float dt)
{
	//if (KEYDOWN("LeftM"))
	//{
	//	--m_iMargin;
	//}
	//if (KEYDOWN("RightM"))
	//{
	//	++m_iMargin;
	//}
	//if (KEYDOWN("Left"))
	//{
	//	--m_iMoneyDrawStartX;
	//}
	//if (KEYDOWN("Right"))
	//{
	//	++m_iMoneyDrawStartX;
	//}
	//if (KEYDOWN("Up"))
	//{
	//	--m_iMoneyDrawStartY;
	//}
	//if (KEYDOWN("Down"))
	//{
	//	++m_iMoneyDrawStartY;
	//}
	//_cprintf("start %d %d, margin %d\n", m_iMoneyDrawStartX, m_iMoneyDrawStartY, m_iMargin);
}

int UIGameManager::Update(float dt)
{
	if (m_bTickStart)
	{
		m_clock->Tick(dt);
	}
    return 0;
}

int UIGameManager::LateUpdate(float dt)
{
    return 0;
}

void UIGameManager::Collision(float dt)
{
}

void UIGameManager::Draw(HDC hdc, float dt)
{
	m_pTimeUI->Draw(hdc, dt);
	DrawCurrentMoney(hdc);
	DrawCurrentClockNeedle(hdc);
	DrawCurrentTime(hdc);
}

void UIGameManager::DrawCurrentMoney(HDC hdc)
{
	vector<int> currentMoney;
	int money = m_pPlayer->GetMoney();
	int digit = 0;
	do {
		digit = money % 10;
		money /= 10;
		currentMoney.push_back(digit);
	} while (money > 0);

	int stX = m_pTimeUI->GetPos().x + m_iMoneyDrawStartX + (m_iMaxDigits - 1) * (m_iNumberWidth + m_iMargin);
	int stY = m_pTimeUI->GetPos().y + m_iMoneyDrawStartY;

	int nDigits = (int)currentMoney.size();
	for (int i = 0; i < nDigits; ++i)
	{
		m_vecMoneyTex[currentMoney[i]]->DrawImageAt(hdc, Pos(stX, stY));
		stX -= (m_iNumberWidth + m_iMargin);
	}
}

void UIGameManager::SetPlayer(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	if (m_pPlayer)
		m_pPlayer->AddRef();
}

void UIGameManager::DrawCurrentClockNeedle(HDC hdc)
{
	int hours = m_clock->GetHours();
	if (hours >= 0 && hours <= 5)
	{
		hours += 24;
	}
	int iCur;
	if (hours >= m_iMaxHours)
	{
		iCur = int(m_vecNeedleTex.size() - 1);
	}
	else {
		iCur = int((hours - 6) / interval);
	}

	m_vecNeedleTex[iCur]->DrawImageAt(hdc, m_pTimeUI->GetPos());
}

void UIGameManager::DrawCurrentTime(HDC hdc)
{
	Pos tPos = m_pTimeUI->GetPos();
	// ��ȭ���������
	tPos.x += 140;
	tPos.y += m_iUpperDisplayStartY;
	m_vecWeekDays[m_clock->GetWeekDays()]->DrawImageAt(hdc, tPos);

	// ��¥
	int days = m_clock->GetDays();
	tPos.x += 50;
	tPos.y += 4;
	RESOURCE_MANAGER->DrawFontsAt(hdc, to_string(days), tPos.x, tPos.y, RIGHT, 2);

	// ���� ����
	tPos = m_pTimeUI->GetPos();
	tPos.x += 105;
	tPos.y += 114;
	m_vecNoon[m_clock->IsAfterNoon()]->DrawImageAt(hdc, tPos);

	// �ð�
	tPos.x += 45;
	tPos.y += 4;
	ostringstream ss;
	ss << m_clock->GetHours() % 13 << ":" << setw(2) << setfill('0') << m_clock->GetMinutes();
	RESOURCE_MANAGER->DrawFontsAt(hdc, ss.str(), tPos.x, tPos.y, RIGHT, 5);
}

UIGameManager::UIGameManager()
{

}

UIGameManager::~UIGameManager()
{
	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pTimeUI);
	Safe_Release_VecList(m_vecMoneyTex);
	Safe_Release_VecList(m_vecNeedleTex);
	Safe_Release_VecList(m_vecWeekDays);
	Safe_Release_VecList(m_vecNoon);
}

void UIGameManager::Clock::Tick(float dt)
{
	m_fTimeCur += dt;
	if (m_fTimeCur >= m_fTimeUnit)
	{
		m_fTimeCur -= m_fTimeUnit;
		minutes += 10;
		if (minutes == 60)
		{
			++hours;
			minutes = 0;
			if (hours == 24)
			{
				++days;
				weekdays = (weekdays + 1) % 7;
				days = days == 31 ? 1 : days;
			}
			hours %= 24;
			bAfterNoon = hours >= 12 ? true : false;
		}
	}
}

#pragma once
#include "../Game.h"

class CTimer
{
private:
	LARGE_INTEGER m_tSecond;
	LARGE_INTEGER m_tTime;
	float m_fDeltaTime;
	float m_fTimeScale;
	float m_fFPS;
	float m_fFPSTime;
	float m_iFrameMax;
	int  m_iFrame;
public:
	float GetDeltaTime() const
	{
		return m_fDeltaTime * m_fTimeScale;
	}
	float GetFPS() const
	{
		return m_fFPS;
	}
	float GetTimeScale() const
	{
		return m_fTimeScale;
	}
	void SetTimeScale(float timeScale)
	{
		m_fTimeScale = timeScale;
	}
	void Update();
	bool Init();
	DECLARE_SINGLE(CTimer)
};


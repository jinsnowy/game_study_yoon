#include "CTimer.h"

DEFINITION_SINGLE(CTimer)
void CTimer::Update()
{
	LARGE_INTEGER tTime;
	QueryPerformanceCounter(&tTime);

	m_fDeltaTime = (tTime.QuadPart - m_tTime.QuadPart) / (float)m_tSecond.QuadPart;
	m_tTime = tTime;
}

bool CTimer::Init()
{
	QueryPerformanceFrequency(&m_tSecond);
	QueryPerformanceCounter(&m_tTime);

	m_fDeltaTime = 0.f;
	m_fTimeScale = 1.f;
	m_fFPS = 0.f;
	m_fFPSTime = 0.f;
	m_iFrameMax = 60;
	m_iFrame = 0;

	return true;
}

CTimer::CTimer()
{
}

CTimer::~CTimer()
{
}

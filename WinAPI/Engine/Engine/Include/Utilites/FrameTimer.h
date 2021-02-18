#pragma once
#include "../framework.h"
#include <chrono>

class FrameTimer
{
	DECLARE_SINGLE(FrameTimer)
public:
	bool Init();
	void SetTimeScale(float timeScale) { m_TimeScale = timeScale; }

	float Peek();
	float Tick();
private:
	std::chrono::steady_clock::time_point m_Tic;
	float m_TimeScale;
};

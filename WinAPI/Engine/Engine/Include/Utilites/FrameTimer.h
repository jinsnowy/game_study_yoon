#pragma once
#include "../framework.h"
#include <chrono>

class FrameTimer
{
	DECLARE_SINGLE(FrameTimer)
public:
	bool Init();
	float Peek();
	float Tick();
private:
	std::chrono::steady_clock::time_point mTic;
};

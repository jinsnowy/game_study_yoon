#pragma once
#include "../framework.h"
#include <chrono>

class FrameTimer
{
	DECLARE_SINGLE(FrameTimer)
public:
	float Peek();
	float Tick();
private:
	std::chrono::steady_clock::time_point mTic;
};

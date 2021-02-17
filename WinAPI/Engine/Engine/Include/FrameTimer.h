#pragma once
#include <chrono>

class FrameTimer
{
public:
	FrameTimer(const FrameTimer&) = delete;
	FrameTimer(const FrameTimer&&) = delete;
	FrameTimer& operator=(const FrameTimer&) = delete;
	FrameTimer& operator=(const FrameTimer&&) = delete;

	FrameTimer();
	float Peek();
	float Tick();
private:
	std::chrono::steady_clock::time_point mTic;
};

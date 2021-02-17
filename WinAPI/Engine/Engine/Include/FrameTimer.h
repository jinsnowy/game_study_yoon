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
	float Tick();
private:
	std::chrono::high_resolution_clock::time_point mTic;
};

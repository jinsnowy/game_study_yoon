#include "FrameTimer.h"

using namespace std::chrono;

FrameTimer::FrameTimer()
    :mTic(high_resolution_clock::now())
{
}

float FrameTimer::Tick()
{
    const auto tic = mTic;
    mTic = high_resolution_clock::now();
    return (duration_cast<duration<float>> (mTic - tic)).count();
}

#include "FrameTimer.h"

using namespace std::chrono;

FrameTimer::FrameTimer()
    :mTic(steady_clock::now())
{
}

FrameTimer::~FrameTimer()
{
}

bool FrameTimer::Init()
{
    mTic = steady_clock::now();
    return true;
}

float FrameTimer::Peek()
{
    return duration_cast<duration<float>>(mTic - steady_clock::now()).count();
}

float FrameTimer::Tick()
{
    const auto tic = mTic;
    mTic = steady_clock::now();
    return (duration_cast<duration<float>> (mTic - tic)).count();
}

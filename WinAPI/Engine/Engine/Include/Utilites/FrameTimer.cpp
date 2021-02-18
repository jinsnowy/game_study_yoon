#include "FrameTimer.h"

using namespace std::chrono;

FrameTimer::FrameTimer()
    :m_Tic(steady_clock::now()),
     m_TimeScale(1.f)
{
}

FrameTimer::~FrameTimer()
{
}

bool FrameTimer::Init()
{
    m_Tic = steady_clock::now();
    return true;
}

float FrameTimer::Peek()
{
    return duration_cast<duration<float>>(m_Tic - steady_clock::now()).count();
}

float FrameTimer::Tick()
{
    const auto tic = m_Tic;
    m_Tic = steady_clock::now();
    return (duration_cast<duration<float>> (m_Tic - tic)).count();
}

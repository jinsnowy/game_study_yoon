#include "FrameTimer.h"
using namespace chrono;

DEFINITION_SINGLE(FrameTimer)

FrameTimer::FrameTimer()
    : 
    m_hWnd(nullptr),
     m_Tic(steady_clock::now()),
     m_TimeScale(1.f)
{
}

FrameTimer::~FrameTimer()
{
}

bool FrameTimer::Init(HWND hWnd)
{
    m_hWnd = hWnd;
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

    float dt = (duration_cast<duration<float>> (m_Tic - tic)).count();
    m_fFPSTime += dt;
    m_iFrame++;

    if (m_fFPSTime >= 1.f)
    {
        m_fFPS = m_iFrame / m_fFPSTime;
        m_fFPSTime = 0.f;
        m_iFrame = 0;

#ifdef _DEBUG
        char strFPS[64] = {};
        sprintf_s(strFPS, "FPS : %.f\n", m_fFPS);
        // _cprintf(strFPS);
        SetWindowText(m_hWnd, strFPS);
        OutputDebugString(strFPS);
#endif // DEBUG
    }
    return dt;
}

float FrameTimer::ScaledTick()
{
   return Tick() * m_TimeScale;
}

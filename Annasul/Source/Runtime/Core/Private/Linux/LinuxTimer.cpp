#include "Linux/LinuxTimer.hpp"

#include "GenericDebug.hpp"

namespace Annasul
{

    void FLinuxTimer::Reset()
    {
        const auto currentTime = std::chrono::high_resolution_clock::now();
        m_deltaTime = 0.0;
        m_fpsCalculatorDeltaTime = 1.0;
        m_fps = 0.0;
        m_timeDilation = 1.0;

        m_baseTime = currentTime;
        m_pausedTime = Duration::zero();
        m_stopTime = currentTime;
        m_prevTime = currentTime;
        m_currTime = currentTime;

        m_lastCalculatorFpsTime = currentTime;
        m_tickCount = 0;

        m_bIsStopped = false;
    }

    void FLinuxTimer::Start()
    {
        const auto startTime = std::chrono::high_resolution_clock::now();

        if (!IsStop())
        {
            FDebug::Get().Log(EDebugLevel::Error, TEXT("Timer is already running!"));
            return;
        }
        m_pausedTime += startTime - m_stopTime;

        m_prevTime = startTime;

        m_stopTime = {};
        m_bIsStopped = false;
    }

    void FLinuxTimer::Stop()
    {
        if (IsStop())
        {
            FDebug::Get().Log(EDebugLevel::Error, TEXT("Timer is already stopped!"));
            return;
        }
        const auto currentTime = std::chrono::high_resolution_clock::now();

        m_stopTime = currentTime;
        m_bIsStopped = true;
    }

    void FLinuxTimer::Tick()
    {
        m_currTime = std::chrono::high_resolution_clock::now();
        const auto deltaTime = DurationToSeconds(m_currTime - m_prevTime);

        m_deltaTime = IsStop() ? 0.0 : deltaTime * m_timeDilation;

        m_prevTime = m_currTime;

        m_tickCount++;

        if (deltaTime > m_fpsCalculatorDeltaTime)
        {
            m_fps = static_cast<double>(m_tickCount) / deltaTime;

            m_tickCount = 0;
            m_lastCalculatorFpsTime = m_currTime;
        }
    }
}


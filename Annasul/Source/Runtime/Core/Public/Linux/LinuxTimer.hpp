
#pragma once

#include "GenericTimer.hpp"

#include <chrono>

namespace Annasul
{

    class FLinuxTimer : public FGenericTimer
    {
    public:

        typedef std::chrono::high_resolution_clock Clock;
        typedef Clock::time_point TimePoint;
        typedef Clock::duration Duration;

    public:

        FORCEINLINE FLinuxTimer() { FLinuxTimer::Reset(); }
        FORCEINLINE ~FLinuxTimer() override = default;

    public:

        void Reset() override;

        void Start() override;

        void Stop() override;

        void Tick() override;

    public:

        FORCEINLINE void SetTimeDilation(double value) override { m_timeDilation = value; }

        [[nodiscard]] FORCEINLINE double TotalTime() const override
        {
            return DurationToSeconds((IsStop() ? m_stopTime : m_currTime) - m_baseTime - m_pausedTime);
        }

        FORCEINLINE void SetFpsCalculatorDeltaTime(double value) { m_fpsCalculatorDeltaTime = value; }

        [[nodiscard]] FORCEINLINE double GetFpsCalculatorDeltaTime() const { return m_fpsCalculatorDeltaTime; }

        // may be negative
        [[nodiscard]] FORCEINLINE double GetDeltaTime() const override { return m_deltaTime; }

        [[nodiscard]] FORCEINLINE double GetFps() const override { return m_fps; }

        [[nodiscard]] FORCEINLINE double GetTimeDilation() const override { return m_timeDilation; }

        [[nodiscard]] FORCEINLINE bool IsStop() const override { return m_bIsStopped; }

    public:

        template<typename InDurationType>
        FORCEINLINE static double DurationToSeconds(const InDurationType& duration)
        {
            return static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()) * SECONDS_PER_COUNT;
        }

    public:

        static constexpr double SECONDS_PER_COUNT = 1.0 / std::chrono::nanoseconds::period::num;

    private:

        double m_deltaTime = 0.0;
        double m_fpsCalculatorDeltaTime = 1.0;
        double m_fps = 0.0;
        double m_timeDilation = 1.0;

        TimePoint m_baseTime{};
        Duration m_pausedTime{};
        TimePoint m_stopTime{};
        TimePoint m_prevTime{};
        TimePoint m_currTime{};

        TimePoint m_lastCalculatorFpsTime{};
        int64 m_tickCount = 0;

        bool m_bIsStopped = false;

    };

    typedef FLinuxTimer FTimer;

}

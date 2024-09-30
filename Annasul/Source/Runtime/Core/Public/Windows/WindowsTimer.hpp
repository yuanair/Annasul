#pragma once

#include "GenericTimer.hpp"

namespace Annasul
{
	
	class FWindowsTimer : public FGenericTimer
	{
	public:
		
		FORCEINLINE FWindowsTimer() { FWindowsTimer::Reset(); }
		
		FORCEINLINE ~FWindowsTimer() override = default;
	
	public:
		
		void Reset() override;
		
		void Start() override;
		
		void Stop() override;
		
		void Tick() override;
	
	public:
		
		FORCEINLINE void SetTimeDilation(double value) override { m_timeDilation = value; }
		
		[[nodiscard]] FORCEINLINE double TotalTime() const override
		{
			return double((IsStop() ? m_stopTime : m_currTime) - m_pausedTime - m_baseTime) * secondsPerCount;
		}
		
		FORCEINLINE void SetFpsCalculatorDeltaTime(double value) { m_fpsCalculatorDeltaTime = value; }
		
		[[nodiscard]] FORCEINLINE double GetFpsCalculatorDeltaTime() const { return m_fpsCalculatorDeltaTime; }
		
		// may be negative
		[[nodiscard]] FORCEINLINE double GetDeltaTime() const override { return m_deltaTime; }
		
		[[nodiscard]] FORCEINLINE double GetFps() const override { return m_fps; }
		
		[[nodiscard]] FORCEINLINE double GetTimeDilation() const override { return m_timeDilation; }
		
		[[nodiscard]] FORCEINLINE bool IsStop() const override { return m_bIsStopped; }
	
	private:
		
		static double secondsPerCount;
	
	private:
		
		double m_deltaTime = 0.0;
		double m_fpsCalculatorDeltaTime = 1.0;
		double m_fps = 0.0;
		double m_timeDilation = 1.0;
		
		int64 m_baseTime = 0;
		int64 m_pausedTime = 0;
		int64 m_stopTime = 0;
		int64 m_prevTime = 0;
		int64 m_currTime = 0;
		
		int64 m_lastCalculatorFpsTime = 0;
		int64 m_tickCount = 0;
		
		bool m_bIsStopped = false;
		
	};
	
	typedef FWindowsTimer FTimer;
	
}

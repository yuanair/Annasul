#include "Windows/WindowsTimer.hpp"
#include "GenericDebug.hpp"

#include "Windows/Windows.hpp"

namespace Annasul
{
	
	int64 QueryPerformanceFrequency()
	{
		int64 countsPerSec = 0;
		::QueryPerformanceFrequency((LARGE_INTEGER *) &countsPerSec);
		return countsPerSec;
	}
	
	double FWindowsTimer::secondsPerCount = 1.0 / (double) QueryPerformanceFrequency();
	
	void FWindowsTimer::Reset()
	{
		int64 curr = 0;
		::QueryPerformanceCounter((LARGE_INTEGER *) &curr);
		m_deltaTime = 0.0;
		m_fpsCalculatorDeltaTime = 1.0;
		m_fps = 0.0;
		m_timeDilation = 1.0;
		
		m_baseTime = curr;
		m_pausedTime = 0;
		m_stopTime = 0;
		m_prevTime = curr;
		m_currTime = 0;
		
		m_lastCalculatorFpsTime = curr;
		m_tickCount = 0;
		
		m_bIsStopped = false;
	}
	
	void FWindowsTimer::Start()
	{
		int64 startTime = 0;
		::QueryPerformanceCounter((LARGE_INTEGER *) &startTime);
		
		if (!IsStop())
		{
			FDebug::Get().Log(EDebugLevel::Error, TEXT("Timer is already running!"));
			return;
		}
		m_pausedTime += (startTime - m_stopTime);
		
		m_prevTime = startTime;
		
		m_stopTime = 0;
		m_bIsStopped = false;
	}
	
	void FWindowsTimer::Stop()
	{
		if (IsStop())
		{
			FDebug::Get().Log(EDebugLevel::Error, TEXT("Timer is already stopped!"));
			return;
		}
		int64 curr = 0;
		::QueryPerformanceCounter((LARGE_INTEGER *) &curr);
		
		m_stopTime = curr;
		m_bIsStopped = true;
	}
	
	void FWindowsTimer::Tick()
	{
		int64_t curr = 0;
		::QueryPerformanceCounter((LARGE_INTEGER *) &curr);
		m_currTime = curr;
		
		m_deltaTime = IsStop() ? 0.0 : double(m_currTime - m_prevTime) * secondsPerCount * m_timeDilation;
		
		m_prevTime = m_currTime;
		
		m_tickCount++;
		double calculatorFpsDeltaTime = double(m_currTime - m_lastCalculatorFpsTime) * secondsPerCount;
		if (calculatorFpsDeltaTime > m_fpsCalculatorDeltaTime)
		{
			m_fps = double(m_tickCount) / calculatorFpsDeltaTime;
			
			m_tickCount = 0;
			m_lastCalculatorFpsTime = m_currTime;
		}
	}
	
	
}


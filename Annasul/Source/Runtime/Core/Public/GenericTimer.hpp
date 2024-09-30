#pragma once

#include "Platform.hpp"

namespace Annasul
{
	
	class FGenericTimer
	{
	public:
		
		FORCEINLINE FGenericTimer() = default;
		FORCEINLINE virtual ~FGenericTimer() = default;
	
	public:
		
		virtual void Reset() = 0;
		
		virtual void Start() = 0;
		
		virtual void Stop() = 0;
		
		virtual void Tick() = 0;
		
		virtual void SetTimeDilation(double value) = 0;
	
	public:
		
		[[nodiscard]]virtual double TotalTime() const = 0;
		
		[[nodiscard]]virtual double GetTimeDilation() const = 0;
		
		[[nodiscard]]virtual double GetDeltaTime() const = 0;
		
		[[nodiscard]]virtual double GetFps() const = 0;
		
		[[nodiscard]]virtual bool IsStop() const = 0;
		
	};
	
}


#if PLATFORM_WINDOWS

#include "Windows/WindowsTimer.hpp"

#else
#error "GenericTimer is not supported on this platform"
#endif

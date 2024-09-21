#pragma once

#include "Platform.hpp"
#include "Container/StringView.hpp"

#include "SourceLocation.hpp"

namespace Annasul
{
	
	enum class EDebugLevel
	{
		Debug,
		Info,
		Warning,
		Error,
		Fatal
	};
	
	FORCEINLINE FStringView ToString(const EDebugLevel level)
	{
		switch (level)
		{
			case EDebugLevel::Debug:
				return TEXT("Debug");
			case EDebugLevel::Info:
				return TEXT("Info");
			case EDebugLevel::Warning:
				return TEXT("Warning");
			case EDebugLevel::Error:
				return TEXT("Error");
			case EDebugLevel::Fatal:
				return TEXT("Fatal");
		}
		return TEXT("Unknown");
	}
	
	class FGenericDebug
	{
	public:
		
		FGenericDebug() = default;
		
		virtual ~FGenericDebug() = default;
	
	public:
		
		virtual void Log(EDebugLevel level, FStringView message, SourceLocation location) = 0;
		
		virtual bool ConditionLog(bool condition, EDebugLevel level, FStringView message, SourceLocation location) = 0;
		
	};
	
}

#if PLATFORM_WINDOWS
	
	#include "Windows/WindowsDebug.hpp"

#elif PLATFORM_LINUX
	#include "Linux/LinuxDebug.hpp"
#elif PLATFORM_MACOS
	#include "MacOS/MacOSDebug.hpp"
#else
	#error "Platform not supported!"
#endif

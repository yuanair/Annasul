#pragma once

#include "Platform.hpp"
#include "Container/StringView.hpp"

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
		
		///
		/// @param level The level of the message to log
		/// @param message The message to log
		virtual void Log(EDebugLevel level, FStringView message) = 0;
		
		///
		/// @param condition condition to check
		/// @param level The level of the message to log
		/// @param message message to log if condition is false
		/// @return condition
		virtual bool ConditionLog(bool condition, EDebugLevel level, FStringView message) = 0;
		
	};
	
}

#ifdef PLATFORM_WINDOWS
	#include "Windows/WindowsDebug.hpp"
#elif PLATFORM_LINUX
#include "Linux/LinuxDebug.hpp"
#elif PLATFORM_MACOS
	#include "MacOS/MacOSDebug.hpp"
#elif PLATFORM_IOS
	#include "IOS/IOSDebug.hpp"
#elif PLATFORM_ANDROID
	#include "Android/AndroidDebug.hpp"
#else
	#error "Platform not supported!"
#endif

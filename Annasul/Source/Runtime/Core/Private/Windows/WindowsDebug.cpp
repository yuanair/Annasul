#include "Windows/WindowsDebug.hpp"

#include "Platform.hpp"

#include "Windows/Windows.hpp"
#include <cstdio>

namespace Annasul
{
	
	
	FWindowsDebug &FWindowsDebug::Get()
	{
		static FWindowsDebug singleton;
		return singleton;
	}
	
	void FWindowsDebug::Log(EDebugLevel level, FStringView message)
	{
		const FStringView levelStr = ToString(level);
		const SIZE_T length = 1 + levelStr.GetSize() + 1 + message.GetSize() + 2;
		const auto debugBuffer = new TCHAR[length];
		if (::swprintf_s(debugBuffer, length, TEXT("[%s] %s") LINE_TERMINATOR, levelStr.GetNullTerminatedData(), message.GetNullTerminatedData()) < 0)
		{
			::OutputDebugString(TEXT("swprintf_s failed") LINE_TERMINATOR);
			return;
		}
		::OutputDebugString(debugBuffer);
	}
	
	bool FWindowsDebug::ConditionLog(bool condition, EDebugLevel level, FStringView message)
	{
		if (condition) return true;
		Log(level, message);
		return false;
	}
	
	void FWindowsDebug::LastErrorLog(EDebugLevel level, FStringView message)
	{
		const HRESULT error = HRESULT_FROM_WIN32(::GetLastError());
		constexpr TCHAR str[] = TEXT("Windows Last Error code: ");
		const SIZE_T length = FStringView(str).GetSize() + 20 + message.GetSize() + 2;
		const auto debugBuffer = new TCHAR[length];
		if (::swprintf_s(debugBuffer, length, TEXT("Windows Last Error code: %l - %s") LINE_TERMINATOR, message.GetNullTerminatedData()) < 0)
		{
			::OutputDebugString(TEXT("swprintf_s failed") LINE_TERMINATOR);
			return;
		}
		Log(level, FStringView(debugBuffer, length));
	}
	
	bool FWindowsDebug::LastErrorConditionLog(bool condition, EDebugLevel level, FStringView message)
	{
		if (condition) return true;
		LastErrorLog(level, message);
		return false;
	}
	
}


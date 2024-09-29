#include "Windows/WindowsDebug.hpp"

#include "Windows/Windows.hpp"
#include <format>

namespace Annasul
{
	
	
	FWindowsDebug &FWindowsDebug::Get()
	{
		static FWindowsDebug singleton;
		return singleton;
	}
	
	void FWindowsDebug::Log(EDebugLevel level, FStringView message, SourceLocation location)
	{
		const FStringView levelStr = ToString(level);
		::OutputDebugStringA(location.file_name());
		::OutputDebugString(
			std::format(
				TEXT("({}, {})[{}] {}") LINE_TERMINATOR, location.line(),
				location.column(), levelStr.GetNullTerminatedData(),
				message.GetNullTerminatedData()).c_str());
		if (level >= EDebugLevel::Warning)
		{
			DebugBreak();
		}
	}
	
	void FWindowsDebug::LastErrorLog(EDebugLevel level, FStringView message, SourceLocation location)
	{
		ErrorLog(level, message, ::GetLastError(), location);
	}
	
	void FWindowsDebug::ErrorLog(EDebugLevel level, FStringView message, uint32 errorCode, SourceLocation location)
	{
		LPTSTR buffer = nullptr;
		auto bufferLength = ::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &buffer,
			0,
			nullptr
		);
		if (bufferLength == 0)
		{
			Log(EDebugLevel::Error, TEXT("FormatMessage failed"), location);
			return;
		}
		Log(level, FStringView(buffer, bufferLength + 1), location);
		::LocalFree(buffer);
	}
	
	bool
	FWindowsDebug::ErrorLogIfFailed(EDebugLevel level, FStringView message, uint32 errorCode, SourceLocation location)
	{
		if (SUCCEEDED(errorCode)) return true;
		ErrorLog(level, message, errorCode, location);
		return false;
	}
	
}


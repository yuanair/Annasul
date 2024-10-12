#include "Linux/LinuxDebug.hpp"

namespace Annasul
{
	FLinuxDebug &FLinuxDebug::Get()
	{
		static FLinuxDebug instance;
		return instance;
	}
	
	void FLinuxDebug::Log(EDebugLevel level, FStringView message, SourceLocation location)
	{
		const FStringView levelStr = ToString(level);
		::printf("%s", location.file_name());
		::wprintf(
			L"(%d, %d)[%s] %s%s", location.line(),
			location.column(), levelStr.GetNullTerminatedData(),
			message.GetNullTerminatedData(), LINE_TERMINATOR
		);
		if (level >= EDebugLevel::Warning) {
			::fflush(stdout);
		}
	}
	
}


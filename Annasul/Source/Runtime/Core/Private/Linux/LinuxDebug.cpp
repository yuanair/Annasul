#include "Linux/LinuxDebug.hpp"

#include <format>

namespace Annasul
{
    FLinuxDebug& FLinuxDebug::Get()
    {
        static FLinuxDebug instance;
        return instance;
    }

    void FLinuxDebug::Log(EDebugLevel level, FStringView message, SourceLocation location)
    {
        const FStringView levelStr = ToString(level);
        ::printf("%s", location.file_name());
        ::wprintf(L"%s",
            std::format(
                TEXT("({}, {})[{}] {}") LINE_TERMINATOR, location.line(),
                location.column(), levelStr.GetNullTerminatedData(),
                message.GetNullTerminatedData()).c_str());
        if (level >= EDebugLevel::Warning)
        {
            ::fflush(stdout);
        }
    }

}


#pragma once

#include "GenericPlatform.hpp"

#define FORCEINLINE __attribute__((always_inline))
#define FORCENOINLINE __attribute__((noinline))

#define LINE_TERMINATOR TEXT("\n")

namespace Annasul
{

    struct FLinuxPlatformTypes : public FGenericPlatformTypes
    {
        typedef uint64       SIZE_T;
        typedef int64        SSIZE_T;
    };

    typedef FLinuxPlatformTypes FPlatformTypes;

    struct FLinuxPlatform : public FGenericPlatform
    {

        static constexpr FPlatformTypes::TCHAR PLATFORM_NAME[] = TEXT("Linux");

        static bool IsDarkMode();

        static bool IsLightMode();

        static bool IsUserAnAdmin();

    };

    typedef FLinuxPlatform FPlatform;

}

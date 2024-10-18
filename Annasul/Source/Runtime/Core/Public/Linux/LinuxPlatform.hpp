#pragma once

#include "GenericPlatform.hpp"

#define FORCEINLINE __attribute__((always_inline)) inline
#define FORCENOINLINE __attribute__((noinline))

#define LINE_TERMINATOR TEXT("\n")
#define LINE_TERMINATOR_CHAR TEXT('\n')

#define END_OF_FILE (-1)

namespace Annasul
{
	
	struct FLinuxPlatformTypes : public FGenericPlatformTypes {
		typedef uint64 SIZE_T;
		typedef int64 SSIZE_T;
		typedef unsigned int UINT;
		typedef unsigned long ULONG;
		typedef signed int INT;
		typedef signed long LONG;
	};
	
	typedef FLinuxPlatformTypes FPlatformTypes;
	
	struct FLinuxPlatform : public FGenericPlatform {
		
		static constexpr FPlatformTypes::TCHAR PLATFORM_NAME[] = TEXT("Linux");
		
		static bool IsDarkMode();
		
		static bool IsLightMode();
		
		static bool IsUserAnAdmin();
		
	};
	
	typedef FLinuxPlatform FPlatform;
	
}

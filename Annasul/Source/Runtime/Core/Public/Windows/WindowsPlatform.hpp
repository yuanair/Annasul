#pragma once

#include "GenericPlatform.hpp"

#define __TEXT(x) L##x
#define TEXT(x) __TEXT(x)

#define FORCEINLINE __forceinline
#define FORCENOINLINE __declspec(noinline)

#define LINE_TERMINATOR TEXT("\r\n")

struct HWND__;
typedef HWND__* HWND;

namespace Annasul {
	
	struct FWindowsPlatformTypes : public FGenericPlatformTypes
	{
#ifdef _WIN64
		typedef unsigned __int64	SIZE_T;
		typedef __int64				SSIZE_T;
#else
		typedef unsigned long		SIZE_T;
	typedef long				SSIZE_T;
#endif
		typedef ::HWND HWND;
		typedef uint16 ATOM;
	};
	typedef FWindowsPlatformTypes FPlatformTypes;
	
}

#pragma once

#include "GenericPlatform.hpp"

#ifdef __GNUC__
	
	#include <_mingw.h>

#endif

#define FORCEINLINE __forceinline
#define FORCENOINLINE __declspec(noinline)

#define LINE_TERMINATOR TEXT("\r\n")

#define CALLBACK    __stdcall

struct HWND__;
typedef HWND__ *HWND;

namespace Annasul
{
	
	struct FWindowsPlatformTypes : public FGenericPlatformTypes
	{
#ifdef _WIN64
		typedef unsigned __int64 SIZE_T;
		typedef __int64 SSIZE_T;
		typedef __int64 INT_PTR, *PINT_PTR;
		typedef unsigned __int64 UINT_PTR, *PUINT_PTR;
		typedef __int64 LONG_PTR, *PLONG_PTR;
		typedef unsigned __int64 ULONG_PTR, *PULONG_PTR;
#else
		typedef unsigned long SIZE_T;
		typedef long SSIZE_T;
		typedef int INT_PTR, *PINT_PTR;
		typedef unsigned int UINT_PTR, *PUINT_PTR;
		typedef long LONG_PTR, *PLONG_PTR;
		typedef unsigned long ULONG_PTR, *PULONG_PTR;
#endif
		typedef ::HWND HWND;
		typedef uint16 ATOM;
		typedef unsigned int UINT;
		typedef unsigned long ULONG;
		typedef signed int INT;
		typedef signed long LONG;
		typedef UINT_PTR WPARAM;
		typedef LONG_PTR LPARAM;
		typedef LONG_PTR LRESULT;
	};
	typedef FWindowsPlatformTypes FPlatformTypes;
	
	class FStringView;
	
	struct FWindowsPlatform : public FGenericPlatform
	{
		enum Key : FPlatformTypes::uint32
		{
			KetClassesRoot = 0x80000000,
			KeyCurrentUser = 0x80000001,
			KeyLocalMachine = 0x80000002,
			KeyUsers = 0x80000003,
			KeyCurrentConfig = 0x80000005,
		};
		
		static constexpr FPlatformTypes::TCHAR PLATFORM_NAME[] = TEXT("Windows");
		
		static bool IsDarkMode();
		
		static bool IsLightMode();
		
		static bool IsUserAnAdmin();
		
		/// 读取注册表值
		static bool
		ReadFromRegedit(Key hKey, FStringView subKey, FStringView subKeyValue, FPlatformTypes::uint32 &value);
		
		/// 读取注册表值
		/// static bool ReadFromRegedit(Key hKey, FStringView subKey, FStringView subKeyValue, StringW &value);
		
		/// 写入注册表值
		static bool WriteToRegedit(Key hKey, FStringView subKey, FStringView subKeyValue, FStringView value);
		
		/// 删除注册表键
		static bool DeleteKeyInRegedit(Key hKey, FStringView subKey);
		
		/// 删除注册表键值
		static bool DeleteKeyValueInRegedit(Key hKey, FStringView subKey, FStringView subKeyValue);
		
		/// 设置开机启动项
		static bool SetBootStartUp(FStringView key, FStringView command);
		
		/// 获取开机启动项
		/// static bool GetBootStartUp(FStringView key, StringW &command);
		
		/// 删除开机启动项
		static bool DeleteBootStartUp(FStringView key);
		
	};
	
	typedef FWindowsPlatform FPlatform;
	
}

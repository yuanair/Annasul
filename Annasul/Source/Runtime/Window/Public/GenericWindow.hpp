#pragma once


#include "Platform.hpp"
#include "Container/StringView.hpp"

#include "GenericWindowClass.hpp"

namespace Annasul
{
	/// window description
	struct FWindowDesc
	{
		FStringView title;
		int32 width;
		int32 height;
		bool fullscreen;
	};
	
	/// window
	class FGenericWindow
	{
	public:
		
		FGenericWindow() = default;
		virtual ~FGenericWindow() = default;
		
	public:
		
		virtual void Create(const FWindowClass& windowClass, const FWindowDesc& desc) = 0;
		virtual void Show() = 0;
		virtual void Hide() = 0;
		virtual void Close() = 0;
		virtual void Destroy() = 0;
		
		virtual bool IsOpen() const = 0;
		
	};
	
}

#ifdef PLATFORM_WINDOWS
#include "Windows/WindowsWindow.hpp"
#elif PLATFORM_LINUX
#include "Linux/LinuxWindow.hpp"
#elif PLATFORM_MACOS
#include "MacOS/MacOSWindow.hpp"
#elif PLATFORM_IOS
#include "IOS/IOSWindow.hpp"
#elif PLATFORM_ANDROID
#include "Android/AndroidWindow.hpp"
#else
#error "Platform not supported!"
#endif


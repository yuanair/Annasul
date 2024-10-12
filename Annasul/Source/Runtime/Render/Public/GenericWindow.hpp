#pragma once


#include "Platform.hpp"
#include "Container/StringView.hpp"

#include "GenericWindowClass.hpp"
#include "Vector.hpp"

namespace Annasul
{
	/// window description
	struct FWindowDesc {
		FStringView title;
		int32 width;
		int32 height;
		bool fullscreen;
	};
	
	/// window
	class FGenericWindow {
	public:
		
		FGenericWindow() = default;
		
		virtual ~FGenericWindow() = default;
	
	public:
		
		virtual void Create(const FWindowClass &windowClass, const FWindowDesc &desc) = 0;
		
		virtual void DragAcceptFiles(bool accept) = 0;
		
		virtual void SetDarkMode(bool darkMode) = 0;
		
		virtual void Show() = 0;
		
		virtual void Hide() = 0;
		
		virtual void Close() = 0;
		
		virtual void Destroy() = 0;
		
		[[nodiscard]] virtual bool IsOpen() const = 0;
	
	public:
		
		virtual void OnCreate() {}
		
		virtual void OnDestroy() {}
		
		virtual void OnMove(int32 x, int32 y) {}
		
		virtual void OnResize(int32 width, int32 height) {}
		
		virtual void OnActive() {}
		
		virtual void OnClickActive() {}
		
		virtual void OnInactive() {}
		
		virtual void OnClose() { Destroy(); }
		
		virtual bool OnQueryEndSession() { return true; }
		
		virtual void OnEndSession() {}
		
		virtual void OnChar(uint64 code) {}
		
		virtual void OnString(FStringView str) {}
		
		virtual void OnDropFile(FStringView file) {}
	
	public:
		
		virtual void SetMinSize(const FVector2i &size) = 0;
		
		[[nodiscard]] virtual FVector2i GetMinSize() const = 0;
		
		virtual void SetMaxSize(const FVector2i &size) = 0;
		
		[[nodiscard]] virtual FVector2i GetMaxSize() const = 0;
		
		virtual void SetInputPoint(const FVector2i &point) = 0;
		
		[[nodiscard]] virtual FVector2i GetInputPoint() const = 0;
		
		virtual void SetPosition(const FVector2i &position) = 0;
		
		[[nodiscard]] virtual FVector2i GetPosition() const = 0;
		
		virtual void SetSize(const FVector2i &size) = 0;
		
		[[nodiscard]] virtual FVector2i GetSize() const = 0;
		
		virtual void SetPositionAndSize(const FVector2i &position, const FVector2i &size) = 0;
		
		[[nodiscard]] virtual FVector4i GetPositionAndSize() const = 0;
		
	};
	
}

#if PLATFORM_WINDOWS

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


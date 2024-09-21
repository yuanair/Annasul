#pragma once


#include "Platform.hpp"
#include "Container/StringView.hpp"

#include "GenericWindowClass.hpp"
#include "Vector.hpp"

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
		
		virtual void Create(const FWindowClass &windowClass, const FWindowDesc &desc) = 0;
		
		virtual void DragAcceptFiles(bool accept) = 0;
		
		virtual void SetDarkMode(bool darkMode) = 0;
		
		virtual void Show() = 0;
		
		virtual void Hide() = 0;
		
		virtual void Close() = 0;
		
		virtual void Destroy() = 0;
		
		[[nodiscard]] virtual bool IsOpen() const = 0;
	
	public:
		
		virtual void OnCreate() = 0;
		
		virtual void OnDestroy() = 0;
		
		virtual void OnMove(int32 x, int32 y) = 0;
		
		virtual void OnResize(int32 width, int32 height) = 0;
		
		virtual void OnActive() = 0;
		
		virtual void OnClickActive() = 0;
		
		virtual void OnInactive() = 0;
		
		virtual void OnClose() = 0;
		
		virtual bool OnQueryEndSession() = 0;
		
		virtual void OnEndSession() = 0;
		
		virtual void OnChar(uint64 code) = 0;
		
		virtual void OnString(FStringView str) = 0;
		
		virtual void OnDropFile(FStringView file) = 0;
	
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


#pragma once


#include "Platform.hpp"
#include "Container/StringView.hpp"

namespace Annasul
{
	
	struct FWindowClassDesc
	{
		FStringView name;
	};
	
	class FGenericWindowClass
	{
	public:
	
		FGenericWindowClass() = default;
		virtual ~FGenericWindowClass() = default;
		
	public:
		
		virtual void Register(const FWindowClassDesc& desc) = 0;
		virtual void Unregister() = 0;
		
		virtual bool IsRegistered() const = 0;
	
	};
	
}

#ifdef PLATFORM_WINDOWS
	#include "Windows/WindowsWindowClass.hpp"
#elif defined(PLATFORM_LINUX)
	#include "Linux/LinuxWindowClass.hpp"
#elif defined(PLATFORM_MACOS)
	#include "MacOS/MacOSWindowClass.hpp"
#else
	#error "Unsupported platform"
#endif

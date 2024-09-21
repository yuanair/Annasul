#pragma once

#include "Platform.hpp"
#include "Container/StringView.hpp"

namespace Annasul
{
	
	class FGenericSocket
	{
	public:
		
		FGenericSocket() = default;
		
		virtual ~FGenericSocket() = default;
	
	public:
		
		virtual void Connect(FStringView hostname, int32 port) = 0;
		
		virtual void Disconnect() = 0;
		
		[[nodiscard]] virtual bool IsConnected() const = 0;
		
	};
	
}

#if PLATFORM_WINDOWS

#include "Windows/WindowsSocket.hpp"

#elif PLATFORM_LINUX
#include "Linux/LinuxSocket.hpp"
#elif PLATFORM_MAC
#include "Mac/MacSocket.hpp"
#else
#error "Unsupported Platform"
#endif

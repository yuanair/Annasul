#pragma once

#include "GenericSocket.hpp"

namespace Annasul
{
	
	class FWindowsSocket : public FGenericSocket
	{
	public:
		
		FWindowsSocket();
		
		~FWindowsSocket() override;
	
	public:
		
		void Connect(FStringView hostname, int32 port) override;
		
		void Disconnect() override;
		
		[[nodiscard]] bool IsConnected() const override;
		
	};
	
	typedef FWindowsSocket FSocket;
	
	
}

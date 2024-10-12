
#pragma once

#include "GenericSocket.hpp"

namespace Annasul
{
	class FLinuxSocket : public FGenericSocket {
	public:
		
		void Connect(FStringView hostname, int32 port) override;
		
		void Disconnect() override;
		
		bool IsConnected() const override;
		
	};
	
	typedef FLinuxSocket FSocket;
	
}
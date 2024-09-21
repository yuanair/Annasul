#include "Windows/WindowsSocket.hpp"

#include "WinSock.hpp"
#include "GenericDebug.hpp"

#include <format>

namespace Annasul
{
	
	FWindowsSocket::FWindowsSocket()
	{
		WSADATA wsaData;
		if (auto result = ::WSAStartup(MAKEWORD(2, 2), &wsaData); 0 != result)
		{
			FDebug::Get().ErrorLog(EDebugLevel::Error, TEXT(""), result);
		}
	}
	
	FWindowsSocket::~FWindowsSocket()
	{
		if (0 != ::WSACleanup())
		{
			FDebug::Get().ErrorLog(EDebugLevel::Error, TEXT(""), WSAGetLastError());
		}
	}
	
	void FWindowsSocket::Connect(FStringView hostname, int32 port)
	{
	
	}
	
	void FWindowsSocket::Disconnect()
	{
	
	}
	
	bool FWindowsSocket::IsConnected() const
	{
		return false;
	}
	
}

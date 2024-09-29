#include "Windows/Windows.hpp"
#include "Platform.hpp"
#include "World.hpp"

namespace Annasul
{
	
	void Quit(int32 exitCode)
	{
		::PostQuitMessage(exitCode);
	}
	
	int32 Main()
	{
		::MSG msg;
		FWorld::Get().OnStart();
		do
		{
			if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			else
			{
				FWorld::Get().OnTick();
			}
		}
		while (msg.message != WM_QUIT);
		FWorld::Get().OnQuit();
		return static_cast<int32>(msg.wParam);
	}
	
}

int WINAPI WinMain(_In_ ::HINSTANCE hInstance, _In_opt_ ::HINSTANCE hPrevInstance, _In_ ::LPSTR lpCmdLine, _In_
                   int nCmdShow)
{
	return Annasul::Main();
}


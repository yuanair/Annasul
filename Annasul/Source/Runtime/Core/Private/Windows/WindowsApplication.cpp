#include "Windows/WindowsApplication.hpp"

#include "Windows/Windows.hpp"

namespace Annasul
{
	
	int32 FWindowsApplication::Loop()
	{
		::MSG msg;
		do
		{
			if (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
			
			}
		}
		while (msg.message != WM_QUIT);
		return static_cast<int32>(msg.wParam);
	}
	
	void FWindowsApplication::Quit(int32 exitCode)
	{
		::PostQuitMessage(exitCode);
	}
	
}



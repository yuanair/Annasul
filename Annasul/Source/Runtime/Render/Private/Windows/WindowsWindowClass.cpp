#include "Windows/WindowsWindowClass.hpp"

#include "Windows/Windows.hpp"
#include "GenericWindow.hpp"
#include "GenericDebug.hpp"

namespace Annasul
{
	
	FWindowsPlatformTypes::LRESULT
	FWindowsWindowClass::WindowProc(FWindowsPlatformTypes::HWND hWnd, FWindowsPlatformTypes::UINT uMsg,
	                                FWindowsPlatformTypes::WPARAM wParam, FWindowsPlatformTypes::LPARAM lParam)
	{
		FWindowsWindow *pThis;
		
		if (uMsg == WM_NCCREATE)
		{
			auto *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
			pThis = reinterpret_cast<FWindowsWindow *>(pCreate->lpCreateParams);
			::SetWindowLongPtrW((HWND) hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
			pThis->SetHandle(hWnd);
		}
		else
		{
			pThis = reinterpret_cast<FWindowsWindow *>(::GetWindowLongPtrW((HWND) hWnd, GWLP_USERDATA));
		}
		if (pThis) return pThis->OnMessage(uMsg, wParam, lParam);
		
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	
	void FWindowsWindowClass::Register(const FWindowClassDesc &desc)
	{
		Unregister();
		::WNDCLASSEX wnd;
		wnd.cbSize = sizeof(wnd);
		wnd.style = 0;
		wnd.lpfnWndProc = &FWindowsWindowClass::WindowProc;
		wnd.cbClsExtra = 0;
		wnd.cbWndExtra = sizeof(FWindowsWindow *);
		wnd.hInstance = nullptr;
		wnd.hIcon = nullptr;
		wnd.hCursor = nullptr;
		wnd.hbrBackground = nullptr;
		wnd.lpszMenuName = nullptr;
		wnd.lpszClassName = desc.name.GetNullTerminatedData();
		wnd.hIconSm = nullptr;
		
		m_atom = ::RegisterClassEx(&wnd);
		if (IsRegistered()) return;
		FDebug::Get().LastErrorLog(EDebugLevel::Error, TEXT("RegisterClassEx failed"));
	}
	
	bool FWindowsWindowClass::IsRegistered() const
	{
		return m_atom != 0;
	}
	
	void FWindowsWindowClass::Unregister()
	{
		if (!CheckAtomNoLog()) return;
		FDebug::Get().LastErrorConditionLog(
			::UnregisterClass(MAKEINTATOM(m_atom), ::GetModuleHandle(nullptr)), EDebugLevel::Error,
			TEXT("UnregisterClass failed")
		);
		m_atom = 0;
	}
	
	bool FWindowsWindowClass::CheckAtom()
	{
		if (IsRegistered()) return true;
		FDebug::Get().Log(EDebugLevel::Error, TEXT("Window class is not registered"));
		m_atom = 0;
		return false;
	}
	
	bool FWindowsWindowClass::CheckAtomNoLog()
	{
		if (IsRegistered()) return true;
		m_atom = 0;
		return false;
	}
}

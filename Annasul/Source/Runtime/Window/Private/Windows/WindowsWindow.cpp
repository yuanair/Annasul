#include "Windows/WindowsWindow.hpp"

#include "Windows/Windows.hpp"
#include "GenericDebug.hpp"

namespace Annasul
{
	void FWindowsWindow::Create(const FWindowClass& windowClass, const FWindowDesc& desc)
	{
		m_hWnd = ::CreateWindowEx(
			0,
			MAKEINTATOM(windowClass.GetAtom()), desc.title.GetNullTerminatedData(),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, desc.width, desc.height,
			nullptr, nullptr,
			nullptr,
			nullptr
		);
		if (m_hWnd != nullptr) return;
		FDebug::Get().LastErrorLog(EDebugLevel::Error, TEXT("CreateWindowEx failed"));
	}
	
	bool FWindowsWindow::IsOpen() const
	{
		return ::IsWindow(m_hWnd);
	}
	
	void FWindowsWindow::Show()
	{
		if (!CheckWindowHandle()) return;
		::ShowWindow(m_hWnd, SW_SHOW);
	}
	
	void FWindowsWindow::Hide()
	{
		if (!CheckWindowHandle()) return;
		::ShowWindow(m_hWnd, SW_HIDE);
	}
	
	void FWindowsWindow::Close()
	{
		if (!CheckWindowHandle()) return;
		FDebug::Get().LastErrorConditionLog(::CloseWindow(m_hWnd), EDebugLevel::Error, TEXT("CloseWindow failed"));
	}
	
	void FWindowsWindow::Destroy()
	{
		if (!CheckWindowHandleNoLog()) return;
		FDebug::Get().LastErrorConditionLog(::DestroyWindow(m_hWnd), EDebugLevel::Error, TEXT("DestroyWindow failed"));
	}
	
	bool FWindowsWindow::CheckWindowHandle()
	{
		if (IsOpen()) return true;
		FDebug::Get().Log(EDebugLevel::Error, TEXT("Invalid window handle"));
		m_hWnd = nullptr;
		return false;
	}
	
	bool FWindowsWindow::CheckWindowHandleNoLog()
	{
		if (IsOpen()) return true;
		m_hWnd = nullptr;
		return false;
	}
}

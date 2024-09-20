#pragma once

#include "GenericWindow.hpp"

namespace Annasul
{
	
	class FWindowsWindow final : public FGenericWindow
	{
	public:
	
		FWindowsWindow() : m_hWnd(nullptr) {}
		~FWindowsWindow() override { FWindowsWindow::Destroy(); }
		
	public:
		
		void Create(const FWindowClass &windowClass, const FWindowDesc &desc) override;
		
		void Show() override;
		
		void Hide() override;
		
		void Close() override;
		
		void Destroy() override;
		
		bool IsOpen() const override;
		
	public:
		
		bool CheckWindowHandle();
		bool CheckWindowHandleNoLog();
		
		FORCEINLINE FWindowsPlatformTypes::HWND GetHandle() const { return m_hWnd; }
	
	private:
	
		FWindowsPlatformTypes::HWND m_hWnd;
	
	};
	
	typedef FWindowsWindow FWindow;
	
}

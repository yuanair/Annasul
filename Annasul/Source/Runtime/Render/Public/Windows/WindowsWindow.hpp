#pragma once

#include "GenericWindow.hpp"
#include "Vector.hpp"
#include <limits>

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

namespace Annasul
{
	
	class FWindowsWindow : public FGenericWindow {
	public:
		
		friend class FWindowsWindowClass;
	
	public:
		
		FWindowsWindow();
		
		~FWindowsWindow() override { FWindowsWindow::Destroy(); }
	
	public:
		
		void Create(const FWindowClass &windowClass, const FWindowDesc &desc) final;
		
		void DragAcceptFiles(bool accept) final;
		
		void SetDarkMode(bool darkMode) final;
		
		void Show() final;
		
		void Hide() final;
		
		void Close() final;
		
		void Destroy() final;
		
		[[nodiscard]] bool IsOpen() const final;
	
	public:
		
		FORCEINLINE void SetMinSize(const FVector2i &size) final { m_minSize = size; }
		
		[[nodiscard]] FORCEINLINE FVector2i GetMinSize() const final { return m_minSize; }
		
		FORCEINLINE void SetMaxSize(const FVector2i &size) final { m_maxSize = size; }
		
		[[nodiscard]] FORCEINLINE FVector2i GetMaxSize() const final { return m_maxSize; }
		
		FORCEINLINE void SetInputPoint(const FVector2i &point) final { m_inputPoint = point; }
		
		[[nodiscard]] FORCEINLINE FVector2i GetInputPoint() const final { return m_inputPoint; }
		
		void SetPosition(const FVector2i &position) final;
		
		[[nodiscard]] FVector2i GetPosition() const final;
		
		void SetSize(const FVector2i &size) final;
		
		[[nodiscard]] FVector2i GetSize() const final;
		
		void SetPositionAndSize(const FVector2i &position, const FVector2i &size) final;
		
		[[nodiscard]] FVector4i GetPositionAndSize() const final;
	
	public:
		
		bool CheckWindowHandle();
		
		bool CheckWindowHandleNoLog();
		
		[[nodiscard]] FORCEINLINE FWindowsPlatformTypes::HWND GetHandle() const { return m_hWnd; }
	
	private:
		
		FORCEINLINE void SetHandle(FWindowsPlatformTypes::HWND hWnd)
		{
			Destroy();
			m_hWnd = hWnd;
		}
		
		FWindowsPlatformTypes::LRESULT
		OnMessage(uint32 uMsg, FWindowsPlatformTypes::WPARAM wParam, FWindowsPlatformTypes::LPARAM lParam);
	
	private:
		
		FWindowsPlatformTypes::HWND m_hWnd;
		
		FVector2i m_minSize = FVector2i{0, 0};
		
		FVector2i m_maxSize = FVector2i{std::numeric_limits<int32>::max(), std::numeric_limits<int32>::max()};
		
		FVector2i m_inputPoint = FVector2i{0, 0};
		
	};
	
	typedef FWindowsWindow FWindow;
	
}

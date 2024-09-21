#include "Windows/WindowsWindow.hpp"

#include "Windows/Windows.hpp"
#include "GenericDebug.hpp"

namespace Annasul
{
	
	FWindowsWindow::FWindowsWindow()
		: m_hWnd(nullptr)
	{
	}
	
	void FWindowsWindow::Create(const FWindowClass &windowClass, const FWindowDesc &desc)
	{
		::CreateWindowEx(
			0,
			MAKEINTATOM(windowClass.GetAtom()), desc.title.GetNullTerminatedData(),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, desc.width, desc.height,
			nullptr, nullptr,
			nullptr,
			this
		);
		if (m_hWnd == nullptr)
		{
			FDebug::Get().LastErrorLog(EDebugLevel::Error, TEXT("CreateWindowEx failed"));
			return;
		}
		if (FPlatform::IsLightMode())
			SetDarkMode(FALSE);
		else
			SetDarkMode(TRUE);
	}
	
	void FWindowsWindow::DragAcceptFiles(bool accept)
	{
		::DragAcceptFiles(m_hWnd, accept);
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
		// m_hWnd = nullptr; // Don't set to nullptr, it sets to null in OnMessage function.
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
	
	FWindowsPlatformTypes::LRESULT
	FWindowsWindow::OnMessage(uint32 uMsg, FWindowsPlatformTypes::WPARAM wParam, FWindowsPlatformTypes::LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_CREATE:
				OnCreate();
				return 0;
			case WM_DESTROY:
				OnDestroy();
				m_hWnd = nullptr;
				return 0;
			case WM_MOVE:
				OnMove(LOWORD(lParam), HIWORD(lParam));
				return 0;
			case WM_SIZE:
				OnResize(LOWORD(lParam), HIWORD(lParam));
				return 0;
			case WM_ACTIVATE:
				switch (LOWORD(wParam))
				{
					case WA_INACTIVE:
						OnInactive();
						return 0;
					case WA_ACTIVE:
						OnActive();
						return 0;
					case WA_CLICKACTIVE:
						OnClickActive();
						return 0;
					default:
						return 0;
				}
			case WM_PAINT:
			{
				auto GetWindowsThemeColor = []() -> COLORREF
				{
					DWORD crColorization;
					BOOL fOpaqueBlend;
					COLORREF theme_color{};
					HRESULT result = DwmGetColorizationColor(&crColorization, &fOpaqueBlend);
					if (result == S_OK)
					{
						BYTE r, g, b;
						r = (crColorization >> 16) % 256;
						g = (crColorization >> 8) % 256;
						b = crColorization % 256;
						theme_color = RGB(r, g, b);
					}
					return theme_color;
				};
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(m_hWnd, &ps);
				// clear the window
				RECT rect;
				GetClientRect(m_hWnd, &rect);
				FillRect(hdc, &rect, CreateSolidBrush(FPlatform::IsDarkMode() ? RGB(32, 32, 32) : RGB(255, 255, 255)));
				
				EndPaint(m_hWnd, &ps);
				return 0;
			}
			case WM_CLOSE:
				OnClose();
				return 0;
			case WM_QUERYENDSESSION:
				return OnQueryEndSession();
			case WM_ENDSESSION:
				OnEndSession();
				return 0;
			case WM_SETCURSOR:
				if (LOWORD(lParam) == HTCLIENT)
				{
					// SetCursor((HCURSOR) hCursor.handle);
					return TRUE;
				}
				return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
			case WM_GETMINMAXINFO:
			{
				((MINMAXINFO *) lParam)->ptMinTrackSize.x = m_minSize.x();
				((MINMAXINFO *) lParam)->ptMinTrackSize.y = m_minSize.y();
				((MINMAXINFO *) lParam)->ptMaxTrackSize.x = m_maxSize.x();
				((MINMAXINFO *) lParam)->ptMaxTrackSize.y = m_maxSize.y();
				return 0;
			}
			case WM_KEYDOWN:
			case WM_KEYUP:
				return 0;
			case WM_MOUSEMOVE:
			{
				return 0;
//				int32 newMouseX = GET_X_LPARAM(lParam);
//				int32 newMouseY = GET_Y_LPARAM(lParam);
//				int32 deltaX = newMouseX - mousePosition.x();
//				int32 deltaY = newMouseY - mousePosition.y();
//				mousePosition.x() = newMouseX;
//				mousePosition.y() = newMouseY;
//				return OnMouseMoved(
//					EventOnMouseMoveArgs{mousePosition.x(), mousePosition.y(), deltaX, deltaY}
//				);
			}
			case WM_DROPFILES:
			{
				auto hDrop = (HDROP) wParam;
				UINT nFileNum = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);
				TCHAR strFileName[MAX_PATH];
				for (int i = 0; i < nFileNum; i++)
				{
					DragQueryFile(hDrop, i, strFileName, MAX_PATH);
					OnDropFile(strFileName);
				}
				DragFinish(hDrop);
				
				InvalidateRect(m_hWnd, NULL, TRUE);
				return 0;
			}
			case WM_CHAR:
				OnChar(wParam);
				return 0;
			case WM_IME_STARTCOMPOSITION:
			{
				HIMC hIMC = ImmGetContext(m_hWnd);
				
				if (hIMC)
				{
					CANDIDATEFORM candidateForm;
					candidateForm.dwIndex = 0;
					candidateForm.dwStyle = CFS_CANDIDATEPOS;
					candidateForm.ptCurrentPos.x = m_inputPoint.x();
					candidateForm.ptCurrentPos.y = m_inputPoint.y();
					ImmSetCandidateWindow(hIMC, &candidateForm);
				}
				else FDebug::Get().Log(EDebugLevel::Error, TEXT("ImmGetContext Failed"));
				
				ImmReleaseContext(m_hWnd, hIMC);
				return 0;
			}
			case WM_IME_COMPOSITION:
			{
				HIMC hIMC;
				DWORD dwSize;
				HGLOBAL hstr;
				LPTSTR lpstr;
				if (lParam & GCS_RESULTSTR)
				{
					hIMC = ImmGetContext(m_hWnd);
					
					if (!hIMC) FDebug::Get().Log(EDebugLevel::Error, TEXT("ImmGetContext Failed"));
					
					// Read the size of the result string.
					dwSize = ImmGetCompositionString(hIMC, GCS_RESULTSTR, nullptr, 0);
					
					// increase buffer size for terminating null CharA,
					//   maybe it is in UNICODE
					dwSize += sizeof(WCHAR);
					
					hstr = GlobalAlloc(GHND, dwSize);
					if (hstr == nullptr) FDebug::Get().LastErrorLog(EDebugLevel::Error, TEXT("GlobalAlloc Failed"));
					
					lpstr = (LPTSTR) GlobalLock(hstr);
					if (lpstr == nullptr) FDebug::Get().LastErrorLog(EDebugLevel::Error, TEXT("GlobalLock Failed"));
					
					// Read the result strings that is generated by IME into lpstr.
					ImmGetCompositionString(hIMC, GCS_RESULTSTR, lpstr, dwSize);
					ImmReleaseContext(m_hWnd, hIMC);
					
					// add this string into text buffer of application
					if (lpstr != nullptr) OnString(FStringView(lpstr, dwSize / sizeof(WCHAR)));
					
					GlobalUnlock(hstr);
					GlobalFree(hstr);
				}
				return 0;
			}
			case WM_MENUCHAR:
				// disable alt-key menu.
				return MAKELRESULT(0, MNC_CLOSE);
			case WM_SETTINGCHANGE:
			case WM_THEMECHANGED:
				if (FPlatform::IsLightMode())
					SetDarkMode(FALSE);
				else
					SetDarkMode(TRUE);
				return 0;
			default:
				return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		}
	}
	
	void FWindowsWindow::SetDarkMode(bool darkMode)
	{
		BOOL useDarkMode = darkMode;
		if (FAILED(::DwmSetWindowAttribute(m_hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode))))
		{
			FDebug::Get().LastErrorLog(EDebugLevel::Error, TEXT("FWindowsWindow::SetDarkMode failed"));
		}
	}
	
	void FWindowsWindow::SetPosition(const FVector2i &position)
	{
		::SetWindowPos(m_hWnd, nullptr, position.x(), position.y(), 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
	}
	
	FVector2i FWindowsWindow::GetPosition() const
	{
		RECT rect;
		::GetWindowRect(m_hWnd, &rect);
		return FVector2i(rect.left, rect.top);
	}
	
	void FWindowsWindow::SetSize(const FVector2i &size)
	{
		::SetWindowPos(m_hWnd, nullptr, 0, 0, size.x(), size.y(), SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
	}
	
	FVector2i FWindowsWindow::GetSize() const
	{
		RECT rect;
		::GetWindowRect(m_hWnd, &rect);
		return FVector2i(rect.right - rect.left, rect.bottom - rect.top);
	}
	
	void FWindowsWindow::SetPositionAndSize(const FVector2i &position, const FVector2i &size)
	{
		::SetWindowPos(m_hWnd, nullptr, position.x(), position.y(), size.x(), size.y(), SWP_NOZORDER | SWP_NOACTIVATE);
	}
	
	FVector4i FWindowsWindow::GetPositionAndSize() const
	{
		RECT rect;
		::GetWindowRect(m_hWnd, &rect);
		return FVector4i(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
	}
	
}

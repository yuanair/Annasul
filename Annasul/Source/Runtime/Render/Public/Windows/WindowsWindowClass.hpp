#pragma once

#include "GenericWindowClass.hpp"

namespace Annasul
{
	class FWindowsWindowClass : public FGenericWindowClass {
	public:
		
		FORCEINLINE FWindowsWindowClass() : m_atom(0) {}
		
		FORCEINLINE ~FWindowsWindowClass() override { FWindowsWindowClass::Unregister(); }
	
	public:
		
		void Register(const FWindowClassDesc &desc) final;
		
		void Unregister() final;
		
		[[nodiscard]] bool IsRegistered() const final;
	
	public:
		
		bool CheckAtom();
		
		bool CheckAtomNoLog();
		
		[[nodiscard]] FORCEINLINE FWindowsPlatformTypes::ATOM GetAtom() const { return m_atom; }
	
	private:
		
		static FWindowsPlatformTypes::LRESULT CALLBACK
		WindowProc(FWindowsPlatformTypes::HWND hWnd, FWindowsPlatformTypes::UINT uMsg,
		           FWindowsPlatformTypes::WPARAM wParam, FWindowsPlatformTypes::LPARAM lParam);
		
		FORCEINLINE void SetAtom(FWindowsPlatformTypes::ATOM atom)
		{
			Unregister();
			m_atom = atom;
		}
	
	private:
		
		FWindowsPlatformTypes::ATOM m_atom;
		
	};
	
	typedef FWindowsWindowClass FWindowClass;
	
}

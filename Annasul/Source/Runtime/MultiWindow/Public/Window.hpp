#pragma once

#include "GenericWindow.hpp"
#include "Actor.hpp"
#include "WindowClass.hpp"

namespace Annasul
{
	
	class AWindow : public AActor, public FWindow
	{
	public:
		
		AWindow();
		~AWindow() override;
	
	public:
		
		[[nodiscard]] FORCEINLINE AWindowClass *GetWindowClass() const { return m_windowClass; }
		
		FORCEINLINE void SetWindowClass(AWindowClass *windowClass) { m_windowClass = windowClass; }
	
	protected:
		
		void OnBeginPlay() override;
		void OnTick(double deltaTime) override;
		void OnEndPlay() override;
	
	private:
		
		AWindowClass *m_windowClass;
		
	};
	
}

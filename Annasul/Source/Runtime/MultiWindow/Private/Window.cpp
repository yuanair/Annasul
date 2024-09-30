#include "Window.hpp"

namespace Annasul
{
	
	AWindow::AWindow()
	{
	
	}
	
	AWindow::~AWindow()
	{
	
	}
	
	void AWindow::OnBeginPlay()
	{
		if (GetWindowClass() == nullptr)
		{
			FDebug::Get().Log(EDebugLevel::Error, TEXT("Window class is not set"));
			return;
		}
		FWindowDesc windowDesc{.title = TEXT("Window"), .width = 800, .height = 600, .fullscreen = false};
		Create(*GetWindowClass(), windowDesc);
	}
	
	void AWindow::OnTick(double deltaTime)
	{
	
	}
	
	void AWindow::OnEndPlay()
	{
		Destroy();
	}
}
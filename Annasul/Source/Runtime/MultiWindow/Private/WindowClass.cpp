#include "WindowClass.hpp"

namespace Annasul
{
	
	AWindowClass::AWindowClass()
	{
	
	}
	
	AWindowClass::~AWindowClass()
	{
	
	}
	
	void AWindowClass::OnBeginPlay()
	{
		FWindowClassDesc windowClassDesc{.name = TEXT("AnnasulWindowClass")};
		Register(windowClassDesc);
	}
	
	void AWindowClass::OnTick(double deltaTime)
	{
	
	}
	
	void AWindowClass::OnEndPlay()
	{
		Unregister();
	}
}


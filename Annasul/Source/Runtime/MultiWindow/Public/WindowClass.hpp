#pragma once

#include "GenericWindowClass.hpp"
#include "Actor.hpp"

namespace Annasul
{
	
	class AWindowClass : public AActor, public FWindowClass
	{
	public:
		
		AWindowClass();
		~AWindowClass() override;
	
	protected:
		
		void OnBeginPlay() override;
		void OnTick(double deltaTime) override;
		void OnEndPlay() override;
		
	};
	
}

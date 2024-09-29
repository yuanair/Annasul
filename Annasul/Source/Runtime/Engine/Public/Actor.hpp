#pragma once

#include "Platform.hpp"

namespace Annasul
{
	
	class AActor
	{
	public:
		
		FORCEINLINE AActor() = default;
		FORCEINLINE virtual ~AActor() = default;
	
	protected:
		
		virtual void OnBeginPlay() = 0;
		
		virtual void OnTick(double deltaTime) = 0;
		
		virtual void OnEndPlay() = 0;
		
	};
	
}

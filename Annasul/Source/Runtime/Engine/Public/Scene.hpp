#pragma once

#include "Platform.hpp"

namespace Annasul
{
	
	class FScene
	{
	public:
		
		FORCEINLINE FScene() = default;
		FORCEINLINE virtual ~FScene() = default;
	
	protected:
		
		virtual void OnBeginPlay() = 0;
		
		virtual void OnTick(double deltaTime) = 0;
		
		virtual void OnEndPlay() = 0;
		
	};
	
}

#pragma once

#include "Platform.hpp"

namespace Annasul
{
	
	class SCSceneComponent
	{
	public:
		
		FORCEINLINE SCSceneComponent() = default;
		FORCEINLINE virtual ~SCSceneComponent() = default;
	
	protected:
		
		virtual void OnBeginPlay() = 0;
		
		virtual void OnTick(double deltaTime) = 0;
		
		virtual void OnEndPlay() = 0;
		
	};
	
}
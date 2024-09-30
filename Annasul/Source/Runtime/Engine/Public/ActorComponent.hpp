#pragma once

#include "Platform.hpp"

namespace Annasul
{
	
	class ACActorComponent
	{
		
		friend class AActor;
	
	public:
		
		FORCEINLINE ACActorComponent() = default;
		FORCEINLINE virtual ~ACActorComponent() = default;
	
	protected:
		
		virtual void OnBeginPlay() = 0;
		
		virtual void OnTick(double deltaTime) = 0;
		
		virtual void OnEndPlay() = 0;
		
	};
	
}

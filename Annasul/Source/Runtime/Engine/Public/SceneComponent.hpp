#pragma once

#include "Platform.hpp"
#include "ActorComponent.hpp"
#include "Transform.hpp"

namespace Annasul
{
	
	class SCSceneComponent : public ACActorComponent {
	public:
		
		FORCEINLINE SCSceneComponent() = default;
		
		FORCEINLINE ~SCSceneComponent() override = default;
	
	private:
		
		FTransform transform;
		
	};
	
}
#pragma once

#include "Platform.hpp"
#include "Container/DynamicArray.hpp"

namespace Annasul
{
	
	class FWorld;
	
	class AActor;
	
	class FLevel {
	public:
		
		friend class FWorld;
	
	public:
		
		enum class ERouteActorInitializationState : uint8 {
			Preinitialize,
			Initialize,
			BeginPlay,
			Finished
		};
	
	public:
		
		FORCEINLINE FLevel() : m_world(nullptr),
		                       m_routeActorInitializationState(ERouteActorInitializationState::Preinitialize) {}
		
		FORCEINLINE virtual ~FLevel() = default;
	
	public:
		
		void RouteActorInitialize();
		
		void Tick(double deltaTime);
		
		void ActorUninitialize();
	
	public:
		
		[[nodiscard]] FORCEINLINE ERouteActorInitializationState
		GetRouteActorInitializationState() const { return m_routeActorInitializationState; }
		
		[[nodiscard]] FORCEINLINE FWorld *GetWorld() const { return m_world; }
	
	public:
		
		TFDynamicArray<AActor *> m_actors;
	
	private:
		
		FORCEINLINE void SetRouteActorInitializationState(
			ERouteActorInitializationState state) { m_routeActorInitializationState = state; }
	
	private:
		
		FWorld *m_world;
		
		ERouteActorInitializationState m_routeActorInitializationState;
		
	};
	
}

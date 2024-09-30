#include "Level.hpp"
#include "World.hpp"
#include "Actor.hpp"

#include "GenericDebug.hpp"

namespace Annasul
{
	
	
	void FLevel::Tick(double deltaTime)
	{
		for (auto actor: m_actors)
		{
			actor->Tick(deltaTime);
		}
	}
	
	void FLevel::RouteActorInitialize()
	{
		switch (GetRouteActorInitializationState())
		{
			case ERouteActorInitializationState::Preinitialize:
				SetRouteActorInitializationState(ERouteActorInitializationState::Initialize);
				break;
			case ERouteActorInitializationState::Initialize:
				SetRouteActorInitializationState(ERouteActorInitializationState::BeginPlay);
				break;
			case ERouteActorInitializationState::BeginPlay:
				if (GetWorld() && GetWorld()->GetWorldStartState() == FWorld::EWorldStartState::Started)
				{
					for (auto actor: m_actors)
					{
						actor->BeginPlay();
					}
				}
				SetRouteActorInitializationState(ERouteActorInitializationState::Finished);
				break;
			case ERouteActorInitializationState::Finished:
				break;
			default:
				FDebug::Get().Log(EDebugLevel::Error, TEXT("Invalid RouteActorInitializationState"));
				break;
		}
	}
	
	void FLevel::ActorUninitialize()
	{
		for (auto actor: m_actors)
		{
			actor->EndPlay();
		}
		m_actors.clear();
	}
}


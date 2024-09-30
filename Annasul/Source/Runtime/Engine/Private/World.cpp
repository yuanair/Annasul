#include "World.hpp"

namespace Annasul
{
	
	FWorld &FWorld::Get()
	{
		static FWorld world;
		return world;
	}
	
	void FWorld::OnStart()
	{
		if (GetWorldStartState() != EWorldStartState::NotStarted)
		{
			FDebug::Get().Log(EDebugLevel::Error, TEXT("World is already started"));
			return;
		}
		SetWorldStartState(EWorldStartState::Starting);
		GetTimer().Reset();
		for (auto &level: GetLevels())
		{
			level->RouteActorInitialize();
		}
		SetWorldStartState(EWorldStartState::Started);
	}
	
	void FWorld::OnTick()
	{
		if (GetWorldStartState() != EWorldStartState::Started)
		{
			FDebug::Get().Log(EDebugLevel::Error, TEXT("World is not started"));
			return;
		}
		GetTimer().Tick();
		auto deltaTime = GetTimer().GetDeltaTime();
		for (auto &level: GetLevels())
		{
			level->Tick(deltaTime);
		}
	}
	
	void FWorld::OnQuit()
	{
		if (GetWorldStartState() != EWorldStartState::Started)
		{
			FDebug::Get().Log(EDebugLevel::Error, TEXT("World is not started"));
			return;
		}
		for (auto &level: GetLevels())
		{
			level->ActorUninitialize();
		}
		m_levels.clear();
	}
	
	void FWorld::AddToWorld(FLevel *level)
	{
		if (GetWorldStartState() != EWorldStartState::Started)
		{
			FDebug::Get().Log(EDebugLevel::Error, TEXT("Cannot create level before world is started."));
			return;
		}
		m_levels.emplace_back(level);
		level->m_world = this;
		do
		{
			level->RouteActorInitialize();
		}
		while (level->GetRouteActorInitializationState() != FLevel::ERouteActorInitializationState::Finished);
	}
	
	void FWorld::RemoveFromWorld(FLevel *level)
	{
		level->ActorUninitialize();
		level->m_world = nullptr;
		m_levels.erase(
			std::remove_if(
				m_levels.begin(), m_levels.end(),
				[level](const std::unique_ptr<FLevel> &ptr) { return ptr.get() == level; }
			), m_levels.end());
	}
	
	
}

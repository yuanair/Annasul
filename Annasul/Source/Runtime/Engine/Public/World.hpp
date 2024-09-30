#pragma once

#include "Platform.hpp"
#include "Level.hpp"
#include "GenericDebug.hpp"
#include "GenericTimer.hpp"

#include <vector>
#include <memory>

#pragma message("TODO: delete this include<vector>")

namespace Annasul
{
	
	class FWorld final
	{
		
		friend int32 Main();
	
	public:
		
		enum class EWorldStartState : uint8
		{
			NotStarted,
			Starting,
			Started,
		};
	
	public:
		
		static FWorld &Get();
	
	public:
		
		void AddToWorld(FLevel *level);
		
		void RemoveFromWorld(FLevel *level);
	
	public:
		
		[[nodiscard]] FORCEINLINE EWorldStartState GetWorldStartState() const { return m_worldStarted; }
		
		[[nodiscard]] FORCEINLINE FTimer &GetTimer() { return m_timer; }
		
		[[nodiscard]] FORCEINLINE const FTimer &GetTimer() const { return m_timer; }
		
		[[nodiscard]] FORCEINLINE auto &GetLevels() const { return m_levels; }
	
	private:
		
		FORCEINLINE FWorld() : m_worldStarted(EWorldStartState::NotStarted) {}
		
		FORCEINLINE ~FWorld() = default;
	
	private:
		
		void OnStart();
		
		void OnTick();
		
		void OnQuit();
	
	private:
		
		FORCEINLINE void SetWorldStartState(EWorldStartState state) { m_worldStarted = state; }
	
	private:
		
		std::vector<std::unique_ptr<FLevel>> m_levels;
		
		FTimer m_timer;
		
		EWorldStartState m_worldStarted;
		
	};
	
	
}

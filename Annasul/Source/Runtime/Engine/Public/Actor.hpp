#pragma once

#include "Platform.hpp"
#include "ActorComponent.hpp"

#include "GenericDebug.hpp"
#include "Container/DynamicArray.hpp"

#include <memory>

namespace Annasul
{
	
	class AActor {
	public:
		
		enum class EActorBeginPlayState : uint8 {
			HasNotBegunPlay,
			BeginningPlay,
			HasBegunPlay,
		};
	
	public:
		
		FORCEINLINE AActor() : m_actorHasBegunPlay(EActorBeginPlayState::HasNotBegunPlay) {}
		
		FORCEINLINE virtual ~AActor() = default;
	
	public:
		
		[[nodiscard]] FORCEINLINE EActorBeginPlayState GetActorHasBegunPlay() const { return m_actorHasBegunPlay; }
		
		[[nodiscard]] FORCEINLINE auto &GetComponents() const { return m_components; }
		
		template<typename InComponentType>
		[[nodiscard]] FORCEINLINE InComponentType *GetComponent() const
		{
			for (auto &component: m_components) {
				if (auto castedComponent = std::dynamic_pointer_cast<InComponentType>(component)) {
					return castedComponent.get();
				}
			}
			return nullptr;
		}
	
	public:
		
		void BeginPlay();
		
		void Tick(double deltaTime);
		
		void EndPlay();
	
	protected:
		
		template<typename InComponentType, typename... Args>
		FORCEINLINE InComponentType *CreateComponentDefault(Args &&... args)
		{
			if (GetActorHasBegunPlay() != EActorBeginPlayState::HasNotBegunPlay) {
				FDebug::Get().Log(EDebugLevel::Error, TEXT("Cannot create component after BeginPlay has been called"));
				return nullptr;
			}
			return static_cast<InComponentType *>(m_components.Emplace_GetRef(
				std::make_unique<InComponentType>(std::forward<Args>(args)...)).get());
		}
	
	protected:
		
		virtual void OnBeginPlay() = 0;
		
		virtual void OnTick(double deltaTime) = 0;
		
		virtual void OnEndPlay() = 0;
	
	private:
		
		FORCEINLINE void SetActorHasBegunPlay(EActorBeginPlayState state) { m_actorHasBegunPlay = state; }
	
	private:
		
		TFDynamicArray<std::unique_ptr<ACActorComponent>> m_components;
		
		EActorBeginPlayState m_actorHasBegunPlay;
		
	};
	
}

#include "Actor.hpp"

namespace Annasul
{
	void AActor::BeginPlay()
	{
		if (GetActorHasBegunPlay() != EActorBeginPlayState::HasNotBegunPlay)
		{
			FDebug::Get().Log(EDebugLevel::Error, TEXT("Actor has already begun play"));
			return;
		}
		SetActorHasBegunPlay(EActorBeginPlayState::BeginningPlay);
		OnBeginPlay();
		for (auto &child: GetComponents())
		{
			child->OnBeginPlay();
		}
		SetActorHasBegunPlay(EActorBeginPlayState::HasBegunPlay);
	}
	
	void AActor::Tick(double deltaTime)
	{
		if (GetActorHasBegunPlay() != EActorBeginPlayState::HasBegunPlay)
		{
			FDebug::Get().Log(EDebugLevel::Error, TEXT("Actor has not begun play yet"));
			return;
		}
		OnTick(deltaTime);
		for (auto &child: GetComponents())
		{
			child->OnTick(deltaTime);
		}
	}
	
	void AActor::EndPlay()
	{
		if (GetActorHasBegunPlay() != EActorBeginPlayState::HasBegunPlay)
		{
			FDebug::Get().Log(EDebugLevel::Error, TEXT("Actor has not begun play yet"));
			return;
		}
		OnEndPlay();
		for (auto &child: GetComponents())
		{
			child->OnEndPlay();
		}
	}
	
}

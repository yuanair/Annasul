
#pragma once

#include "Platform.hpp"
#include "Container/String.hpp"

namespace Annasul
{
	
	class FDefaultLexerStateMachine;
	
	class FDefaultLexerState {
	public:
		
		FORCEINLINE FDefaultLexerState() = default;
		
		FORCEINLINE virtual ~FDefaultLexerState() = default;
	
	public:
		
		virtual void OnEnter(FDefaultLexerStateMachine &stateMachine) = 0;
		
		virtual void OnChar(FDefaultLexerStateMachine &stateMachine, int32 character) = 0;
		
		virtual void OnLeave(FDefaultLexerStateMachine &stateMachine) = 0;
		
	};
	
	class FDefaultLexerStateStart : public FDefaultLexerState {
	public:
		void OnEnter(FDefaultLexerStateMachine &stateMachine) override;
		
		void OnChar(FDefaultLexerStateMachine &stateMachine, int32 character) override;
		
		void OnLeave(FDefaultLexerStateMachine &stateMachine) override;
	};
	
	class FDefaultLexerStateIdentifier : public FDefaultLexerState {
	public:
		void OnEnter(FDefaultLexerStateMachine &stateMachine) override;
		
		void OnChar(FDefaultLexerStateMachine &stateMachine, int32 character) override;
		
		void OnLeave(FDefaultLexerStateMachine &stateMachine) override;
	
	public:
		
		FString buffer;
		
	};
	
	
}

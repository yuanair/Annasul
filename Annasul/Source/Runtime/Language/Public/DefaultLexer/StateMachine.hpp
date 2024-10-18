
#pragma once

#include "Platform.hpp"

namespace Annasul
{
	
	class FDefaultLexer;
	
	class FDefaultLexerStateMachine {
	public:
		
		FORCEINLINE FDefaultLexerStateMachine(FDefaultLexer &lexer, FDefaultLexerState &initialState)
			: m_lexer(lexer), m_currentState(&initialState) {}
		
		FORCEINLINE ~FDefaultLexerStateMachine() = default;
	
	public:
		
		FORCEINLINE void OnChar(int32 character) { m_currentState->OnChar(*this, character); }
	
	public:
		
		FORCEINLINE void SetState(FDefaultLexerState &state)
		{
			m_currentState->OnLeave(*this);
			m_currentState = &state;
			m_currentState->OnEnter(*this);
		}
		
		[[nodiscard]] FORCEINLINE const FDefaultLexerState &GetState() const { return *m_currentState; }
		
		FDefaultLexer &GetLexer() { return m_lexer; }
		
		[[nodiscard]] const FDefaultLexer &GetLexer() const { return m_lexer; }
	
	private:
		
		FDefaultLexer &m_lexer;
		FDefaultLexerState *m_currentState;
		
	};
	
}

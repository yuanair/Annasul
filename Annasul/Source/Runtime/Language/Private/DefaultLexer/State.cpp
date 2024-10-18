#include "DefaultLexer/State.hpp"
#include "DefaultLexer/Tokens.hpp"
#include "DefaultLexer.hpp"
#include "Locale.hpp"
#include <cstdio>

namespace Annasul
{
	
	
	void FDefaultLexerStateStart::OnEnter(FDefaultLexerStateMachine &stateMachine)
	{
	
	}
	
	void FDefaultLexerStateStart::OnChar(FDefaultLexerStateMachine &stateMachine, int32 character)
	{
		if (FLocale::IsSpace(character)) {
		
		} else if (FLocale::IsAlpha(character)) {
			stateMachine.SetState(stateMachine.GetLexer().stateIdentifier);
			stateMachine.GetLexer().stateIdentifier.buffer += character;
		} else if (character == END_OF_FILE) {
			stateMachine.GetLexer().Token<FEOFToken>();
		} else {
			stateMachine.GetLexer().Error(
				FDefaultLexer::Error_InvalidCharacter, TEXT("Invalid character at start of file")
			);
		}
	}
	
	void FDefaultLexerStateStart::OnLeave(FDefaultLexerStateMachine &stateMachine)
	{
	
	}
	
	void FDefaultLexerStateIdentifier::OnEnter(FDefaultLexerStateMachine &stateMachine)
	{
	
	}
	
	void FDefaultLexerStateIdentifier::OnChar(FDefaultLexerStateMachine &stateMachine, int32 character)
	{
		if (FLocale::IsAlpha(character) || FLocale::IsDigit(character)) {
			buffer += character;
		} else if (character == END_OF_FILE) {
			stateMachine.GetLexer().Token<FEOFToken>();
			stateMachine.SetState(stateMachine.GetLexer().stateStart);
		} else {
			stateMachine.SetState(stateMachine.GetLexer().stateStart);
			stateMachine.GetLexer().stateStart.OnChar(stateMachine, character);
		}
	}
	
	void FDefaultLexerStateIdentifier::OnLeave(FDefaultLexerStateMachine &stateMachine)
	{
		stateMachine.GetLexer().Token<FIdentifierToken>(buffer);
		buffer = {};
	}
}


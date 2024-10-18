#include "DefaultLexer.hpp"

namespace Annasul
{
	
	void FDefaultLexer::Lex(int32 character)
	{
		if (character == LINE_TERMINATOR_CHAR) {
			++tokenPos.line;
			tokenPos.column = 0;
		}
		++tokenPos.column;
		m_stateMachine.OnChar(character);
	}
	
	FDefaultLexer::FDefaultLexer()
		: m_stateMachine(*this, stateStart), tokenPos(0, 0)
	{
		++tokenPos.line;
	}
	
	void FDefaultLexer::Error(FDefaultLexer::EErrorType type, const FString &message)
	{
		errors.Emplace(tokenPos, type, message);
	}
	
	void FDefaultLexer::Warning(FDefaultLexer::EWarningType type, const FString &message)
	{
		warnings.Emplace(tokenPos, type, message);
	}
	
	void FDefaultLexer::Info(FDefaultLexer::EInfoType type, const FString &message)
	{
		infos.Emplace(tokenPos, type, message);
	}
	
	void FDefaultLexer::Reset()
	{
		tokens.Reset();
		errors.Reset();
		warnings.Reset();
		infos.Reset();
		tokenPos = {1, 0};
		m_stateMachine.SetState(stateStart);
	}
	
	
}


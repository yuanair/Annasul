#pragma once

#include "Lexer.hpp"

#include "Utility.hpp"
#include "Platform.hpp"
#include "Container/String.hpp"
#include "DefaultLexer/Token.hpp"
#include "DefaultLexer/State.hpp"
#include "DefaultLexer/StateMachine.hpp"

#include <memory>

namespace Annasul
{
	
	class FDefaultLexer : public ILexer {
	public:
		
		enum EErrorType {
			Error_InvalidCharacter = 0x100,
		};
		
		enum EWarningType {
		
		};
		
		enum EInfoType {
		
		};
		
		struct FError {
			FTokenPos pos;
			EErrorType type;
			FString message;
		};
		
		struct FWarning {
			FTokenPos pos;
			EWarningType type;
			FString message;
		};
		
		struct FInfo {
			FTokenPos pos;
			EInfoType type;
			FString message;
		};
		
		struct FTokenAndPos {
			FTokenPos pos;
			std::unique_ptr<FToken> token;
		};
	
	public:
		
		FDefaultLexer();
		
		FORCEINLINE ~FDefaultLexer() override = default;
	
	public:
		
		void Lex(int32 character);
		
		void Reset();
		
		template<typename InTokenType, typename... Args>
		FORCEINLINE void Token(Args &&... args)
		{
			tokens.Emplace(tokenPos, std::make_unique<InTokenType>(std::forward<Args>(args)...));
		}
		
		void Error(EErrorType type, const FString &message);
		
		void Warning(EWarningType type, const FString &message);
		
		void Info(EInfoType type, const FString &message);
	
	public:
		
		TFDynamicArray<FTokenAndPos> tokens;
		TFDynamicArray<FError> errors;
		TFDynamicArray<FWarning> warnings;
		TFDynamicArray<FInfo> infos;
	
	public:
		
		FTokenPos tokenPos;
	
	public:
		
		FDefaultLexerStateStart stateStart;
		FDefaultLexerStateIdentifier stateIdentifier;
	
	private:
		
		FDefaultLexerStateMachine m_stateMachine;
		
	};
	
	
}

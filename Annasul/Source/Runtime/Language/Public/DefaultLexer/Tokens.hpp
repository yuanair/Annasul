
#pragma once

#include "Token.hpp"

namespace Annasul
{
	
	class FEOFToken : public FToken {
	public:
		
		FORCEINLINE FEOFToken() = default;
		
		FORCEINLINE ~FEOFToken() override = default;
	
	public:
		
		[[nodiscard]] FString ToString() const override { return TEXT("EOF"); }
		
	};
	
	class FKeywordToken : public FToken {
	public:
		
		FORCEINLINE explicit FKeywordToken(FString keyword) : m_keyword(std::move(keyword)) {}
		
		FORCEINLINE ~FKeywordToken() override = default;
	
	public:
		
		[[nodiscard]] FString ToString() const override { return m_keyword; }
	
	public:
		
		FString m_keyword;
		
	};
	
	class FIdentifierToken : public FToken {
	public:
		
		FORCEINLINE explicit FIdentifierToken(FString identifier) : m_identifier(std::move(identifier)) {}
		
		FORCEINLINE ~FIdentifierToken() override = default;
	
	public:
		
		[[nodiscard]] FString ToString() const override { return m_identifier; }
	
	public:
		FString m_identifier;
		
	};
	
	class FNumberToken : public FToken {
	public:
		
		FORCEINLINE explicit FNumberToken(FString number) : m_number(std::move(number)) {}
		
		FORCEINLINE ~FNumberToken() override = default;
	
	public:
		
		[[nodiscard]] FString ToString() const override { return m_number; }
	
	public:
		FString m_number;
		
	};
	
	class FStringToken : public FToken {
	public:
		
		FORCEINLINE explicit FStringToken(FString string) : m_string(std::move(string)) {}
		
		FORCEINLINE ~FStringToken() override = default;
	
	public:
		FString m_string;
		
	};
	
	class FCharToken : public FToken {
	public:
		
		FORCEINLINE explicit FCharToken(FString inChar) : m_char(std::move(inChar)) {}
		
		FORCEINLINE ~FCharToken() override = default;
	
	public:
		FString m_char;
		
	};
	
	class FOperatorToken : public FToken {
	public:
		
		FORCEINLINE explicit FOperatorToken(FString inOperator) : m_operator(std::move(inOperator)) {}
		
		FORCEINLINE ~FOperatorToken() override = default;
	
	public:
		FString m_operator;
		
	};
	
}

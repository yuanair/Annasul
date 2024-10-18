#include <Platform.hpp>
#include <GenericTimer.hpp>
#include <Reflect.hpp>
#include <iostream>
#include <iomanip>
#include <Container/DynamicArray.hpp>
#include <Container/String.hpp>
#include <DefaultLexer.hpp>

using namespace Annasul;

std::wostream &operator<<(std::wostream &os, const FString &array)
{
	os << array.GetData();
	return os;
}

template<typename InElementType>
std::wostream &operator<<(std::wostream &os, const TFDynamicArray<InElementType> &array)
{
	os << "{ ";
	if (array.GetSize() != 0) {
		os << array[0];
		for (typename TFDynamicArray<InElementType>::SizeType i = 1; i < array.GetSize(); ++i) {
			os << ", " << array[i];
		}
	}
	os << " }";
	return os;
}

int main()
{
	
	std::wcout << Annasul::FPlatform::PLATFORM_NAME << " " << std::endl
	           << "Is User Admin: " << Annasul::FPlatform::IsUserAnAdmin() << std::endl;
	{
		FDefaultLexer lexer;
		Annasul::FTimer timer;
		TFDynamicArray<int32_t> array = {1, 2, 3, 4, 5};
		FString str = TEXT("Hello, world!");
		std::wcout << str << "Find: " << array.Find(1) << ", Array: " << array << std::endl;
		
		timer.Tick();
		
		std::wcout << "Time taken by function: " << timer.GetDeltaTime() << " seconds" << std::endl;
		
		while (true) {
			FString input;
			{
				std::wstring _input;
				std::wcout << "> ";
				std::getline(std::wcin, _input);
				input = FString(_input.c_str(), static_cast<int32>(_input.size()));
			}
			if (input == FString(TEXT("exit"), 4)) {
				break;
			}
			for (auto character: input) {
				lexer.Lex(character);
			}
			lexer.Lex(END_OF_FILE);
			for (auto &token: lexer.tokens) {
				std::wcout << TEXT("(") << token.pos.line << TEXT(", ") << token.pos.column << TEXT("): ")
				           << token.token->ToString() << std::endl;
			}
			std::wcout << std::hex;
			for (auto &token: lexer.errors) {
				std::wcout << TEXT("E") << std::setw(4) << std::setfill(TEXT('0')) << token.type << TEXT(": ")
				           << token.message << std::endl;
			}
			for (auto &token: lexer.warnings) {
				std::wcout << TEXT("W") << std::setw(4) << std::setfill(TEXT('0')) << token.type << TEXT(": ")
				           << token.message << std::endl;
			}
			for (auto &token: lexer.infos) {
				std::wcout << TEXT("I") << std::setw(4) << std::setfill(TEXT('0')) << token.type << TEXT(": ")
				           << token.message << std::endl;
			}
			lexer.Reset();
			std::wcout.clear();
		}
		
	}
	
	return 0;
}

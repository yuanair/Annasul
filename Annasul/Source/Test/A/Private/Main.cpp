#include <GenericPlatform.hpp>
#include <GenericTimer.hpp>
#include <Reflect.hpp>
#include <iostream>
#include <chrono>
#include "Container/DynamicArray.hpp"
#include "Container/String.hpp"

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
	
	Annasul::FTimer timer;
	TFDynamicArray<int32_t> array = {1, 2, 3, 4, 5};
	FString str = TEXT("Hello, world!");
	std::wcout << str << "Find: " << array.Find(1) << ", Array: " << array << std::endl;
	
	timer.Tick();
	
	std::wcout << "Time taken by function: " << timer.GetDeltaTime() << " seconds" << std::endl;
	
	return 0;
}

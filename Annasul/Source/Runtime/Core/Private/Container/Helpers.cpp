#include "Container/Helpers.hpp"
#include "GenericDebug.hpp"
#include <format>

namespace Annasul::Private
{
	
	
	void OnInvalidArrayNum(unsigned long long newNum)
	{
		FDebug::Get().Log(
			EDebugLevel::Error, std::format(TEXT("Trying to resize TArray to an invalid size of {}"), newNum));
		for (;;);
	}
	
	void OnInvalidHeapAllocatorNum(int64 NewNum, SIZE_T NumBytesPerElement)
	{
		FDebug::Get().Log(
			EDebugLevel::Error,
			std::format(
				TEXT("Trying to resize TFHeapAllocator to an invalid size of {} with element size {}"),
				NewNum, NumBytesPerElement
			));
		for (;;);
	}
}


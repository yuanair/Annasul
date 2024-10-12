#include "Container/Helpers.hpp"
#include "GenericDebug.hpp"

namespace Annasul::Private
{
	
	
	void OnInvalidArrayNum(unsigned long long newNum)
	{
		FDebug::Get().Log(
			EDebugLevel::Error, TEXT("Trying to resize TArray to an invalid size")
		);
		for (;;);
	}
	
	void OnInvalidHeapAllocatorNum(int64 NewNum, SIZE_T NumBytesPerElement)
	{
		FDebug::Get().Log(
			EDebugLevel::Error, TEXT("Trying to resize TFHeapAllocator to an invalid size")
		);
		for (;;);
	}
}


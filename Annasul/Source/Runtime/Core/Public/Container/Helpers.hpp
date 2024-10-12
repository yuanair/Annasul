
#pragma once

#include "Platform.hpp"

namespace Annasul
{
	
	enum { INDEX_NONE = -1 };
	
	namespace Private
	{
		
		[[noreturn]] void OnInvalidArrayNum(unsigned long long newNum);
		
		[[noreturn]] void OnInvalidHeapAllocatorNum(int64 NewNum, SIZE_T NumBytesPerElement);
		
	}
}
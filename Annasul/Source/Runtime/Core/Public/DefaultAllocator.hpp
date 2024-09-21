#pragma once

#include "Allocator.hpp"
#include "GenericMemory.hpp"

namespace Annasul
{
	
	struct FDefaultAllocator : public FAllocator
	{
		
		using SizeType = uint64;
		
		[[nodiscard]] FORCEINLINE SizeType GetInitialCapacity() const
		{
			return 0;
		}
		
		FORCEINLINE void *
		Allocate(const int32 numElements, const SIZE_T numBytesPerElement, const uint32 alignmentOfElement)
		{
			return FMemory::Malloc(numElements * numBytesPerElement, alignmentOfElement);
		}
		
		FORCEINLINE void *
		Realloc(void *ptr, const int32 numElements, const SIZE_T numBytesPerElement, const uint32 alignmentOfElement)
		{
			return FMemory::Realloc(ptr, numElements * numBytesPerElement, alignmentOfElement);
		}
		
		FORCEINLINE void Free(void *ptr)
		{
			FMemory::Free(ptr);
		}
		
	};
}
#pragma once

#include "Platform.hpp"
#include "GenericMemory.hpp"
#include <limits>
#include <cassert>

namespace Annasul
{
	
	template<typename SizeType>
	FORCEINLINE SizeType DefaultCalculateSlackShrink(SizeType numElements, SizeType numAllocatedElements,
	                                                 SIZE_T bytesPerElement, bool bAllowQuantize,
	                                                 uint32 alignment)
	{
		SizeType result;
		checkSlow(numElements < numAllocatedElements);
		
		const SizeType CurrentSlackElements = numAllocatedElements - numElements;
		const SIZE_T CurrentSlackBytes = (numAllocatedElements - numElements) * bytesPerElement;
		const bool bTooManySlackBytes = CurrentSlackBytes >= 16384;
		const bool bTooManySlackElements = 3 * numElements < 2 * numAllocatedElements;
		if ((bTooManySlackBytes || bTooManySlackElements) &&
		    (CurrentSlackElements > 64 || !numElements)) {
			result = numElements;
			if (result > 0) {
				if (bAllowQuantize) {
					result = (SizeType) (result * bytesPerElement) / bytesPerElement;
				}
			}
		} else {
			result = numAllocatedElements;
		}
		
		return result;
	}
	
	template<typename SizeType>
	FORCEINLINE SizeType DefaultCalculateSlackGrow(SizeType numElements, SizeType numAllocatedElements,
	                                               SIZE_T bytesPerElement, bool bAllowQuantize,
	                                               uint32 alignment)
	{
		const SIZE_T FirstGrow = 4;
		const SIZE_T ConstantGrow = 16;
		
		SizeType result;
		assert(numElements > numAllocatedElements && numElements > 0);
		
		SIZE_T grow = FirstGrow;
		
		if (numAllocatedElements) {
			grow = SIZE_T(numElements) + 3 * SIZE_T(numElements) / 8 + ConstantGrow;
		} else if (SIZE_T(numElements) > grow) {
			grow = SIZE_T(numElements);
		}
		
		if (bAllowQuantize) {
			result = (SizeType) (grow * bytesPerElement / bytesPerElement);
		} else {
			result = (SizeType) grow;
		}
		if (numElements > result) {
			result = std::numeric_limits<SizeType>::max();
		}
		
		return result;
	}
	
	template<typename SizeType>
	FORCEINLINE SizeType DefaultCalculateSlackReserve(SizeType numElements, SIZE_T bytesPerElement, bool bAllowQuantize,
	                                                  uint32 alignment)
	{
		SizeType result = numElements;
		assert(numElements > 0);
		if (bAllowQuantize) {
			result = (SizeType) ((SIZE_T(result) * SIZE_T(bytesPerElement)) / bytesPerElement);
			if (numElements > result) result = std::numeric_limits<SizeType>::max();
		}
		
		return result;
	}
	
	struct FAllocator {
	public:
		
		FORCEINLINE void Allocate(const int32 numElements, const SIZE_T numBytesPerElement,
		                          const uint32 alignmentOfElement) = delete;
		
		FORCEINLINE void Realloc(const int32 numElements, const SIZE_T numBytesPerElement,
		                         const uint32 alignmentOfElement) = delete;
		
		FORCEINLINE void Free() = delete;
		
	};
}

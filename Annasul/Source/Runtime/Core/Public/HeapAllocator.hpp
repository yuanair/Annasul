#pragma once

#include "Allocator.hpp"
#include "GenericMemory.hpp"

namespace Annasul
{
	
	namespace Private
	{
		[[noreturn]] void OnInvalidHeapAllocatorNum(int64 NumElements, SIZE_T NumBytesPerElement);
	}
	
	template<typename InIndexType>
	class TFHeapAllocator : public FAllocator {
	public:
		
		using SizeType = InIndexType;
		using USizeType = std::make_unsigned_t<SizeType>;
	
	public:
		
		FORCEINLINE TFHeapAllocator() : m_ptr(nullptr) {}
		
		FORCEINLINE ~TFHeapAllocator()
		{
			if (m_ptr) FMemory::Free(m_ptr);
		}
	
	public:
		
		[[nodiscard]] FORCEINLINE SizeType GetInitialCapacity() const
		{
			return 0;
		}
		
		void ResizeAllocation(SizeType PreviousNumElements, SizeType NumElements, SIZE_T NumBytesPerElement)
		{
			if (m_ptr || NumElements) {
				static_assert(sizeof(SizeType) <= sizeof(SIZE_T), "SIZE_T is expected to handle all possible sizes");
				
				// Check for under/overflow
				bool bInvalidResize =
					NumElements < 0 || NumBytesPerElement < 1 ||
					NumBytesPerElement > (SIZE_T) std::numeric_limits<int32>::max();
				if constexpr (sizeof(SizeType) == sizeof(SIZE_T)) {
					bInvalidResize = bInvalidResize || (SIZE_T) (USizeType) NumElements >
					                                   (SIZE_T) std::numeric_limits<SizeType>::max() /
					                                   NumBytesPerElement;
				}
				if (bInvalidResize) {
					Private::OnInvalidHeapAllocatorNum(NumElements, NumBytesPerElement);
				}
				m_ptr = FMemory::Realloc(m_ptr, NumElements * NumBytesPerElement);
			}
		}
		
		void ResizeAllocation(SizeType PreviousNumElements, SizeType NumElements, SIZE_T NumBytesPerElement,
		                      uint32 AlignmentOfElement)
		{
			if (m_ptr || NumElements) {
				static_assert(sizeof(SizeType) <= sizeof(SIZE_T), "SIZE_T is expected to handle all possible sizes");
				
				// Check for under/overflow
				bool bInvalidResize =
					NumElements < 0 || NumBytesPerElement < 1 ||
					NumBytesPerElement > (SIZE_T) std::numeric_limits<int32>::max();
				if constexpr (sizeof(SizeType) == sizeof(SIZE_T)) {
					bInvalidResize = bInvalidResize || ((SIZE_T) (USizeType) NumElements >
					                                    (SIZE_T) std::numeric_limits<SizeType>::max() /
					                                    NumBytesPerElement);
				}
				if (bInvalidResize) {
					Private::OnInvalidHeapAllocatorNum(NumElements, NumBytesPerElement);
				}
				m_ptr = FMemory::Realloc(m_ptr, NumElements * NumBytesPerElement, AlignmentOfElement);
			}
		}
		
		FORCEINLINE SizeType CalculateSlackReserve(SizeType NumElements, SIZE_T NumBytesPerElement) const
		{
			return DefaultCalculateSlackReserve(NumElements, NumBytesPerElement, true);
		}
		
		FORCEINLINE SizeType CalculateSlackReserve(SizeType NumElements, SIZE_T NumBytesPerElement,
		                                           uint32 AlignmentOfElement) const
		{
			return DefaultCalculateSlackReserve(NumElements, NumBytesPerElement, true, (uint32) AlignmentOfElement);
		}
		
		FORCEINLINE SizeType CalculateSlackShrink(SizeType NumElements, SizeType NumAllocatedElements,
		                                          SIZE_T NumBytesPerElement) const
		{
			return DefaultCalculateSlackShrink(NumElements, NumAllocatedElements, NumBytesPerElement, true);
		}
		
		FORCEINLINE SizeType CalculateSlackShrink(SizeType NumElements, SizeType NumAllocatedElements,
		                                          SIZE_T NumBytesPerElement, uint32 AlignmentOfElement) const
		{
			return DefaultCalculateSlackShrink(
				NumElements, NumAllocatedElements, NumBytesPerElement, true, (uint32) AlignmentOfElement
			);
		}
		
		FORCEINLINE SizeType CalculateSlackGrow(SizeType NumElements, SizeType NumAllocatedElements,
		                                        SIZE_T NumBytesPerElement) const
		{
			return DefaultCalculateSlackGrow(NumElements, NumAllocatedElements, NumBytesPerElement, true);
		}
		
		FORCEINLINE SizeType CalculateSlackGrow(SizeType NumElements, SizeType NumAllocatedElements,
		                                        SIZE_T NumBytesPerElement, uint32 AlignmentOfElement) const
		{
			return DefaultCalculateSlackGrow(
				NumElements, NumAllocatedElements, NumBytesPerElement, true, (uint32) AlignmentOfElement
			);
		}
		
		template<typename OtherAllocator>
		FORCEINLINE void MoveToEmptyFromOtherAllocator(OtherAllocator &other)
		{
			if (m_ptr) FMemory::Free(m_ptr);
			m_ptr = other.m_ptr;
			other.m_ptr = nullptr;
		}
		
		FORCEINLINE void MoveToEmpty(TFHeapAllocator &Other)
		{
			this->MoveToEmptyFromOtherAllocator<TFHeapAllocator>(Other);
		}
		
		[[nodiscard]] FORCEINLINE void *GetData() { return m_ptr; }
		
		[[nodiscard]] FORCEINLINE const void *GetData() const { return m_ptr; }
	
	private:
		
		void *m_ptr;
		
	};
	
	typedef TFHeapAllocator<int64> FHeapAllocator;
	
}

#include "DynamicArray.hpp"
#include "GenericMemory.hpp"

namespace Annasul
{
	
	template<typename InElementType, typename InAllocatorType>
	TFDynamicArray<InElementType, InAllocatorType>::TFDynamicArray()
		: m_data(nullptr), m_allocator(), m_size(0), m_capacity(m_allocator.GetInitialCapacity())
	{
		m_data = m_allocator.Allocate(m_capacity);
	}
	
	template<typename InElementType, typename InAllocatorType>
	TFDynamicArray<InElementType, InAllocatorType>::TFDynamicArray(const TFDynamicArray &other)
		: m_data(other.m_allocator.Allocate(other.m_capacity)), m_allocator(other.m_allocator), m_size(other.m_size),
		  m_capacity(other.m_capacity)
	{
		CopyToEmpty(other.m_data, other.m_size);
	}
	
	template<typename InElementType, typename InAllocatorType>
	TFDynamicArray<InElementType, InAllocatorType>::TFDynamicArray(TFDynamicArray &&other)
	{
		MoveOrCopy(*this, other, 0);
	}
	
	template<typename InElementType, typename InAllocatorType>
	TFDynamicArray<InElementType, InAllocatorType>::~TFDynamicArray()
	{
		FMemory::DestructItems(m_data, m_size);
	}
	
}

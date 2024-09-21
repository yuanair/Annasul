#pragma once

#include "GenericPlatform.hpp"
#include "Utility.hpp"

namespace Annasul
{
	
	template<typename InElementType, typename InAllocatorType>
	class TFDynamicArray
	{
	public:
		
		using ElementType = InElementType;
		using AllocatorType = InAllocatorType;
		using SizeType = typename InAllocatorType::SizeType;
	
	public:
		
		TFDynamicArray();
		
		TFDynamicArray(const TFDynamicArray &other);
		
		TFDynamicArray(TFDynamicArray &&other);
		
		~TFDynamicArray();
	
	public:
		
		FORCEINLINE TFDynamicArray &operator=(TFDynamicArray other)
		{
			other.swap(*this);
			return *this;
		}
		
		FORCEINLINE ElementType &operator[](SizeType index) { return At(index); }
		
		FORCEINLINE const ElementType &operator[](SizeType index) const { return At(index); }
		
		FORCEINLINE bool operator==(const TFDynamicArray &other) const { return Equals(other); }
		
		FORCEINLINE bool operator!=(const TFDynamicArray &other) const { return !Equals(other); }
		
		FORCEINLINE TFDynamicArray &operator+=(ElementType element)
		{
			Append(element);
			return *this;
		}
		
		FORCEINLINE TFDynamicArray &operator+=(const TFDynamicArray &other)
		{
			Append(other);
			return *this;
		}
		
		FORCEINLINE TFDynamicArray &operator+=(TFDynamicArray &&other)
		{
			Append(std::move(other));
			return *this;
		}
	
	public:
		
		TFDynamicArray &Assign(TFDynamicArray other) {}
		
		ElementType &At(SizeType index);
		const ElementType &At(SizeType index) const;
		
		bool Equals(const TFDynamicArray &other) const;
		
		TFDynamicArray &Append(ElementType element);
		TFDynamicArray &Append(const TFDynamicArray &other);
		TFDynamicArray &Append(TFDynamicArray &&other);
		
		void Clear();
		
		void Reserve(SizeType newCapacity);
	
	public:
		
		void swap(TFDynamicArray &other)
		{
			using std::swap;
			swap(m_data, other.m_data);
			swap(m_allocator, other.m_allocator);
			swap(m_size, other.m_size);
			swap(m_capacity, other.m_capacity);
		}
	
	public:
		
		[[nodiscard]] AllocatorType &GetAllocator() { return m_allocator; }
		
		[[nodiscard]] const AllocatorType &GetAllocator() const { return m_allocator; }
		
		[[nodiscard]] SizeType GetCapacity() const { return m_capacity; }
		
		[[nodiscard]] SizeType GetSize() const { return m_size; }
		
		[[nodiscard]] ElementType *GetData() { return m_data; }
		
		[[nodiscard]] const ElementType *GetData() const { return m_data; }
		
		[[nodiscard]] bool IsEmpty() const { return m_size == 0; }
	
	private:
		
		AllocatorType m_allocator;
		SizeType m_capacity;
		SizeType m_size;
		ElementType *m_data;
		
	};
	
	
}

#include "DynamicArray.inl"

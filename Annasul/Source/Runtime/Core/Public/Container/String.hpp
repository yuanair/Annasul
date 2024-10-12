#pragma once

#include "DynamicArray.hpp"

namespace Annasul
{
	
	class FString {
	public:
		
		typedef TFDynamicArray<TCHAR> ArrayType;
		typedef ArrayType::AllocatorType AllocatorType;
		typedef ArrayType::ElementType ElementType;
		typedef ArrayType::SizeType SizeType;
	
	public:
		
		FORCEINLINE FString() : m_data() {}
		
		FORCEINLINE FString(std::initializer_list<ElementType> list) : m_data(list) {}
		
		template<SizeType InSize>
		FORCEINLINE FString(const TCHAR (&str)[InSize]): m_data(str) {} // NOLINT(*-explicit-constructor)
		
		FORCEINLINE FString(const TCHAR *str, SizeType num) : m_data(str, num) {}
		
		FORCEINLINE FString(const FString &other) = default;
		
		FORCEINLINE FString(FString &&other) noexcept: m_data(std::move(other.m_data)) {}
		
		FORCEINLINE ~FString() = default;
	
	public:
		
		FORCEINLINE FString &operator=(const FString &other) = default;
		
		FORCEINLINE FString &operator=(FString &&other) noexcept
		{
			m_data = std::move(other.m_data);
			return *this;
		}
		
		FORCEINLINE ElementType &operator[](SizeType index) { return m_data[index]; }
		
		FORCEINLINE const ElementType &operator[](SizeType index) const { return m_data[index]; }
		
		FORCEINLINE bool operator==(const FString &other) const { return m_data == other.m_data; }
		
		FORCEINLINE bool operator!=(const FString &other) const { return m_data != other.m_data; }
		
		FORCEINLINE FString &operator+=(ElementType element)
		{
			m_data += element;
			return *this;
		}
		
		FORCEINLINE FString &operator+=(const FString &other)
		{
			m_data += other.m_data;
			return *this;
		}
		
		FORCEINLINE FString &operator+=(FString &&other)
		{
			m_data += std::move(other.m_data);
			return *this;
		}
		
		FORCEINLINE FString &operator+=(std::initializer_list<ElementType> list)
		{
			m_data += list;
			return *this;
		}
		
		template<SizeType InSize>
		FORCEINLINE FString &operator+=(const TCHAR (&str)[InSize])
		{
			m_data += str;
			return *this;
		}
	
	public:
		
		FORCEINLINE ArrayType &GetArray() { return m_data; }
		
		FORCEINLINE const ArrayType &GetArray() const { return m_data; }
		
		FORCEINLINE ElementType *GetData() { return m_data.GetData(); }
		
		FORCEINLINE const ElementType *GetData() const { return m_data.GetData(); }
		
		FORCEINLINE SizeType GetSize() const { return m_data.GetSize(); }
		
		FORCEINLINE SizeType GetCapacity() const { return m_data.GetCapacity(); }
		
		FORCEINLINE AllocatorType &GetAllocator() { return m_data.GetAllocator(); }
		
		FORCEINLINE const AllocatorType &GetAllocator() const { return m_data.GetAllocator(); }
		
		FORCEINLINE bool IsEmpty() const { return m_data.IsEmpty(); }
	
	private:
		
		ArrayType m_data;
		
	};
	
}
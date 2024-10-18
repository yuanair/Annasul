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
		
		FORCEINLINE FString &operator=(std::initializer_list<ElementType> initList)
		{
			m_data = initList;
			return *this;
		}
		
		template<SizeType InSize>
		FORCEINLINE FString &operator=(const ElementType (&data)[InSize])
		{
			m_data = data;
			return *this;
		}
		
		FORCEINLINE FString &operator=(const FString &other)
		{
			m_data = other.m_data;
			return *this;
		}
		
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
		
		FORCEINLINE auto begin() { return m_data.begin(); }
		
		[[nodiscard]] FORCEINLINE auto begin() const { return m_data.begin(); }
		
		FORCEINLINE auto end() { return m_data.end(); }
		
		[[nodiscard]] FORCEINLINE auto end() const { return m_data.end(); }
		
		FORCEINLINE auto rbegin() { return m_data.rbegin(); }
		
		[[nodiscard]] FORCEINLINE auto rbegin() const { return m_data.rbegin(); }
		
		FORCEINLINE auto rend() { return m_data.rend(); }
		
		[[nodiscard]] FORCEINLINE auto rend() const { return m_data.rend(); }
	
	public:
		
		FORCEINLINE ArrayType &GetArray() { return m_data; }
		
		[[nodiscard]] FORCEINLINE const ArrayType &GetArray() const { return m_data; }
		
		FORCEINLINE ElementType *GetData() { return m_data.GetData(); }
		
		[[nodiscard]] FORCEINLINE const ElementType *GetData() const { return m_data.GetData(); }
		
		[[nodiscard]] FORCEINLINE SizeType GetSize() const { return m_data.GetSize(); }
		
		[[nodiscard]] FORCEINLINE SizeType GetCapacity() const { return m_data.GetCapacity(); }
		
		FORCEINLINE AllocatorType &GetAllocator() { return m_data.GetAllocator(); }
		
		[[nodiscard]] FORCEINLINE const AllocatorType &GetAllocator() const { return m_data.GetAllocator(); }
		
		[[nodiscard]] FORCEINLINE bool IsEmpty() const { return m_data.IsEmpty(); }
	
	private:
		
		ArrayType m_data;
		
	};
	
}
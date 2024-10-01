#pragma once

#include "Utility.hpp"
#include "SourceLocation.hpp"

#include <cassert>

namespace Annasul
{
	
	template<typename InElementType, typename InAllocatorType>
	class TFArrayView {
	public:
		
		using ElementType = InElementType;
		using AllocatorType = InAllocatorType;
		using SizeType = typename InAllocatorType::SizeType;
	
	public:
		
		static constexpr auto NPOS{static_cast<SizeType>(-1)};
	
	public:
		
		template<SizeType InSize>
		FORCEINLINE constexpr TFArrayView(const ElementType (&data)[InSize]) // NOLINT(*-explicit-constructor)
			: m_data(data), m_size(InSize)
		{
		}
		
		FORCEINLINE constexpr TFArrayView(const ElementType *data, const SizeType size)
			: m_data(data), m_size(size)
		{
		}
		
		FORCEINLINE constexpr TFArrayView(const TFArrayView &other) = default;
		
		FORCEINLINE constexpr TFArrayView(TFArrayView &&other) = default;
		
		FORCEINLINE constexpr ~TFArrayView() = default;
	
	public:
		
		FORCEINLINE constexpr TFArrayView &operator=(const TFArrayView &other) = default;
		
		FORCEINLINE constexpr TFArrayView &operator=(TFArrayView &&other) = default;
		
		FORCEINLINE constexpr ElementType &operator[](const SizeType index) const
		{
			assert(m_data != nullptr && "called on a null pointer array view");
			assert(index < m_size && "Index out of range");
			return m_data[index];
		}
		
		FORCEINLINE constexpr const ElementType &operator*() const
		{
			assert(m_data != nullptr && "called on a null pointer array view");
			assert(m_size > 0 && "called on an empty array view");
			return *m_data;
		}
		
		FORCEINLINE constexpr auto operator<=>(const TFArrayView &other) const
		{
			return Compare(other) <=> 0;
		}
	
	public:
		
		FORCEINLINE constexpr ElementType &At(const SizeType index) const
		{
			assert(m_data != nullptr && "called on a null pointer array view");
			assert(index < m_size && "Index out of range");
			return m_data[index];
		}
		
		FORCEINLINE constexpr SizeType Find(InputArgumentType<ElementType> element, const SizeType offset = 0) const
		{
			if (m_data == nullptr) return NPOS;
			for (SizeType i = offset; i < m_size; ++i) {
				if (m_data[i] == element) {
					return i;
				}
			}
			return NPOS;
		}
		
		FORCEINLINE constexpr SizeType RFind(InputArgumentType<ElementType> element, SizeType offset = NPOS) const
		{
			if (m_data == nullptr) return NPOS;
			if (m_size <= 0) return NPOS;
			if (offset == NPOS) offset = m_size - 1;
			for (SizeType i = offset; i > 0; --i) {
				if (m_data[i] == element) {
					return i;
				}
			}
			if (m_data[0] == element) return 0;
			return NPOS;
		}
		
		FORCEINLINE constexpr TFArrayView SubView(const SizeType start, const SizeType length) const
		{
			assert(m_data != nullptr && "called on a null pointer array view");
			assert(start + length <= m_size && "Substring out of range");
			return TFArrayView(m_data + start, length);
		}
		
		FORCEINLINE constexpr FPlatformTypes::INT Compare(const TFArrayView &other) const
		{
			if (m_data == nullptr || other.m_data == nullptr) return -1;
			if (m_data == other.m_data) return 0;
			if (m_size < other.m_size) return -1;
			if (m_size > other.m_size) return 1;
			for (SizeType i = 0; i < m_size; ++i) {
				if (m_data[i] < other.m_data[i]) return -1;
				if (m_data[i] > other.m_data[i]) return 1;
			}
			return 0;
		}
	
	public:
		
		[[nodiscard]] FORCEINLINE constexpr const ElementType *GetData() const
		{
			return m_data;
		}
		
		[[nodiscard]] FORCEINLINE constexpr const ElementType *GetNullTerminatedData() const
		{
			assert(RFind(ElementType{}) != NPOS &&
			       "TFArrayView::GetNullTerminatedData() called on a non-null-terminated array view ");
			return m_data;
		}
		
		[[nodiscard]] FORCEINLINE constexpr SizeType GetSize() const
		{
			return m_size;
		}
	
	private:
		
		const ElementType *m_data;
		
		SizeType m_size;
		
	};
	
}


#pragma once

#include <cassert>

namespace Annasul
{
	
	template<typename InElementType, typename InAllocatorType>
	class TFArrayView
	{
	public:
		
		using ElementType = InElementType;
		using AllocatorType = InAllocatorType;
		using SizeType = typename InAllocatorType::SizeType;
	
	public:
		
		template<SizeType InSize>
		TFArrayView(const ElementType (&data)[InSize])
			: m_data(data), m_size(InSize)
		{
		}
		
		TFArrayView(const ElementType *data, SizeType size)
			: m_data(data), m_size(size)
		{
		}
		
		TFArrayView(const TFArrayView &other) = default;
		
		~TFArrayView() = default;
		
		TFArrayView &operator=(const TFArrayView &other) = default;
	
	public:
		[[nodiscard]] const ElementType *GetData() const
		{
			return m_data;
		}
		
		[[nodiscard]] const ElementType *GetNullTerminatedData() const
		{
			//assert(m_size > 0 && m_data[m_size - 1] == ElementType{} && "TFArrayView::GetNullTerminatedData() called on a non-null-terminated array view ");
			return m_data;
		}
		
		[[nodiscard]] SizeType GetSize() const
		{
			return m_size;
		}
	
	private:
		
		const ElementType *m_data;
		
		SizeType m_size;
		
	};
	
}


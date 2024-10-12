#pragma once

#include "ArrayView.hpp"
#include "Platform.hpp"
#include "HeapAllocator.hpp"

#include <string>

namespace Annasul
{
	
	class FStringView : public TFArrayView<TCHAR, FHeapAllocator> {
	public:
		
		
		template<SizeType InSize>
		FORCEINLINE constexpr FStringView(const ElementType (&data)[InSize]) // NOLINT(*-explicit-constructor)
			: TFArrayView(data)
		{
		}
		
		FORCEINLINE FStringView(const std::basic_string<TCHAR> &str) // NOLINT(*-explicit-constructor)
			: TFArrayView(str.data(), str.size() + 1)
		{
		
		}
		
		FORCEINLINE FStringView(const ElementType *data, SizeType size) : TFArrayView(
			data, size
		) {}
		
		FORCEINLINE constexpr FStringView(const TFArrayView &other)
			: TFArrayView(other) {}
		
		FORCEINLINE constexpr FStringView(TFArrayView &&other)
			: TFArrayView(std::move(other)) {}
		
		FORCEINLINE constexpr ~FStringView() = default;
	
	public:
		
		[[nodiscard]] FORCEINLINE constexpr FStringView SubView(const SizeType start, const SizeType length) const
		{
			return TFArrayView::SubView(start, length);
		}
		
	};
	
}

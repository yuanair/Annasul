#pragma once

#include "ArrayView.hpp"
#include "Platform.hpp"
#include "DefaultAllocator.hpp"

#include <string>

namespace Annasul
{
	
	class FStringView : public TFArrayView<TCHAR, FDefaultAllocator> {
	public:
		
		
		template<SizeType InSize>
		FORCEINLINE constexpr FStringView(const ElementType (&data)[InSize]) // NOLINT(*-explicit-constructor)
			: TFArrayView<TCHAR, FDefaultAllocator>(data)
		{
		}
		
		FORCEINLINE FStringView(const std::basic_string<TCHAR> &str) // NOLINT(*-explicit-constructor)
			: TFArrayView<TCHAR, FDefaultAllocator>(str.data(), str.size() + 1)
		{
		
		}
		
		FORCEINLINE FStringView(const ElementType *data, SizeType size) : TFArrayView<TCHAR, FDefaultAllocator>(
			data, size
		) {}
		
		FORCEINLINE constexpr FStringView(const TFArrayView<TCHAR, FDefaultAllocator> &other)
			: TFArrayView<TCHAR, FDefaultAllocator>(other) {}
		
		FORCEINLINE constexpr FStringView(TFArrayView<TCHAR, FDefaultAllocator> &&other)
			: TFArrayView<TCHAR, FDefaultAllocator>(std::move(other)) {}
		
		FORCEINLINE constexpr ~FStringView() = default;
	
	public:
		
		[[nodiscard]] FORCEINLINE constexpr FStringView SubView(const SizeType start, const SizeType length) const
		{
			return TFArrayView::SubView(start, length);
		}
		
	};
	
}

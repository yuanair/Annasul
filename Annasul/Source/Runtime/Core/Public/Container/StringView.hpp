#pragma once

#include "ArrayView.hpp"
#include "Platform.hpp"
#include "DefaultAllocator.hpp"

#include <string>

namespace Annasul
{
	
	class FStringView : public TFArrayView<TCHAR, FDefaultAllocator>
	{
	public:
		
		
		template<SizeType InSize>
		FStringView(const ElementType (&data)[InSize])
			: TFArrayView<TCHAR, FDefaultAllocator>(data)
		{
		}
		
		FStringView(const std::basic_string<TCHAR> &str)
			: TFArrayView<TCHAR, FDefaultAllocator>(str.data(), str.size() + 1)
		{
		
		}
		
		FStringView(const ElementType *data, SizeType size) : TFArrayView<TCHAR, FDefaultAllocator>(data, size) {}
		
		FStringView(const FStringView &other) = default;
		~FStringView() = default;
		
	};
	
}

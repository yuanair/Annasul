#pragma once
#include "ArrayView.hpp"
#include "Platform.hpp"
#include "DefaultAllocator.hpp"

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
		
		FStringView(const ElementType* data, SizeType size) : TFArrayView<TCHAR, FDefaultAllocator>(data, size) {}
		
		FStringView(const FStringView& other) = default;
		~FStringView() = default;
		
	};
	
}

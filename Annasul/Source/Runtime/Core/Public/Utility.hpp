#pragma once

#include "Platform.hpp"

#include <utility>

namespace Annasul
{
	
	template<typename InType>
	struct InputArgument {
		static_assert(
			!std::is_reference_v<InType>,
			"Input argument cannot be a reference type"
		);
		using Type = std::conditional_t<std::is_integral_v<InType>, std::add_const_t<InType>, std::add_lvalue_reference_t<std::add_const_t<InType>>>;
	};
	
	template<typename InType>
	using InputArgumentType = typename InputArgument<InType>::Type;
	
	template<typename InType>
	struct OutputArgument {
		static_assert(
			!std::is_reference_v<InType> && !std::is_const_v<InType>,
			"Output argument cannot be a reference type or const type"
		);
		using Type = InType &;
	};
	
	template<typename InType>
	using OutputArgumentType = typename OutputArgument<InType>::Type;
	
	template<typename InType>
	struct InOutArgument {
		static_assert(
			!std::is_reference_v<InType> && !std::is_const_v<InType>,
			"InOut argument cannot be a reference type or const type"
		);
		using Type = InType &;
	};
	
	template<typename InType>
	using InOutArgumentType = typename InOutArgument<InType>::Type;
	
}
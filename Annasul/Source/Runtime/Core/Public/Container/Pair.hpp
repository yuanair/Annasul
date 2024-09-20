#pragma once

#include "Utility.hpp"

namespace Annasul
{
	template<typename InType1, typename InType2>
	class TFPair
	{
	public:
		
		TFPair(InType1 first, InType2 second) : first(std::move(first)), second(std::move(second)) {}
		
		TFPair(const TFPair& other) = default;
		TFPair(TFPair&& other) = default;
		
		~TFPair() = default;
		
	public:
		
		TFPair& operator=(const volatile TFPair&) = delete;
		
		TFPair& operator=(const TFPair& other) {
			if (this == std::addressof(other)) return *this;
			first = other.first;
			second = other.second;
			return *this;
		}
		
		TFPair& operator=(TFPair&& other) {
			if (this == std::addressof(other)) return *this;
			first = std::move(other.first);
			second = std::move(other.second);
			return *this;
		}
		
		bool operator==(const TFPair& other) const {
			return first == other.first && second == other.second;
		}
		
		bool operator!=(const TFPair& other) const {
			return first!= other.first || second!= other.second;
		}
		
	public:
		
		void swap(TFPair& other) {
			using std::swap;
			std::swap(first, other.first);
			std::swap(second, other.second);
		}
		
	public:
		
		InType1 first;
		InType2 second;
		
	};
	
	template<typename InType1, typename InType2>
	TFPair<InType1, InType2> MakePair(InType1&& first, InType2&& second) {
		return TFPair<InType1, InType2>(std::forward<InType1>(first), std::forward<InType2>(second));
	}
}

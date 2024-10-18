
#pragma once

#include "Platform.hpp"
#include "Container/String.hpp"

namespace Annasul
{
	
	struct FTokenPos {
		int32 line;
		int32 column;
	};
	
	class FToken {
	public:
		
		FORCEINLINE FToken() = default;
		
		FORCEINLINE virtual ~FToken() = default;
	
	public:
		
		[[nodiscard]] virtual FString ToString() const = 0;
		
	};
}

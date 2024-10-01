#pragma once

#include "Platform.hpp"
#include <type_traits>

#define A_CLASS_BODY(className) \
public:                         \
static_assert(std::is_class_v<className>, "className must be a class type or struct type"); \
FORCEINLINE static FStringView GetStaticName() { return TEXT(#className); }                 \
virtual FClass& GetClass() const override { return GetStaticName(); }               \
                                \
namespace Annasul
{
	
	class FReflect final {
	public:
		
		static FReflect &Get();
	
	public:
	
	
	private:
		
		FORCEINLINE FReflect() = default;
		
		FORCEINLINE ~FReflect() = default;
	
	private:
	
		
	};
	
	class FClass final {
	public:
	
	
	private:
		
		FORCEINLINE FClass() = default;
		
		FORCEINLINE ~FClass() = default;
		
	};
	
}

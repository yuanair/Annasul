#pragma once

#include "Platform.hpp"

namespace Annasul
{
	
	struct FGenericMemory
	{

		static void* Malloc(SIZE_T count, uint32 alignment) = delete;
		static void* Realloc(void* original, SIZE_T count, uint32 alignment) = delete;
		static void Free(void* original) = delete;
		static void *Copy(void *dest, const void *src, const SIZE_T size) = delete;
		static void *Move(void *dest, const void *src, const SIZE_T size) = delete;
		
	};
	
	template<typename ElementType>
	FORCEINLINE static void DestructItem(ElementType *element)
	{
		typedef ElementType DestructItemsElementTypeTypedef;
		element->DestructItemsElementTypeTypedef::~DestructItemsElementTypeTypedef();
	}
	
	template<typename ElementType, typename SizeType>
	FORCEINLINE static void DestructItems(ElementType *element, SizeType count)
	{
		while (count)
		{
			typedef ElementType DestructItemsElementTypeTypedef;
			
			element->DestructItemsElementTypeTypedef::~DestructItemsElementTypeTypedef();
			++element;
			--count;
		}
	}
	
	template <typename DestinationElementType, typename SourceElementType, typename SizeType>
	FORCEINLINE void ConstructItems(void* dest, const SourceElementType* source, SizeType count)
	{
		while (count)
		{
			new (dest) DestinationElementType(*source);
			++(DestinationElementType*&)dest;
			++source;
			--count;
		}
	}
	
	template <typename ElementType, typename SizeType>
	FORCEINLINE void CopyAssignItems(ElementType* dest, const ElementType* source, SizeType count)
	{
		while (count)
		{
			*dest = *source;
			++dest;
			++source;
			--count;
		}
	}
	
	template <typename DestinationElementType, typename SourceElementType, typename SizeType>
	FORCEINLINE void RelocateConstructItems(void* dest, const SourceElementType* source, SizeType count)
	{
		while (count) {
			typedef SourceElementType RelocateConstructItemsElementTypeTypedef;
			
			new(dest) DestinationElementType(*source);
			++(DestinationElementType * &)
			dest;
			(source++)->RelocateConstructItemsElementTypeTypedef::~RelocateConstructItemsElementTypeTypedef();
			--count;
		}
	}
	
	
	template <typename ElementType, typename SizeType>
	FORCEINLINE void MoveConstructItems(void* dest, const ElementType* source, SizeType count)
	{
		while (count)
		{
			new (dest) ElementType((ElementType&&)*source);
			++(ElementType*&)dest;
			++source;
			--count;
		}
	}
	
	template <typename ElementType, typename SizeType>
	FORCEINLINE void MoveAssignItems(ElementType* dest, const ElementType* source, SizeType count)
	{
		while (count)
		{
			*dest = (ElementType&&)*source;
			++dest;
			++source;
			--count;
		}
	}
	
}

#if PLATFORM_WINDOWS
	
	#include "Windows/WindowsMemory.hpp"

#elif PLATFORM_LINUX
	#include "Linux/LinuxMemory.hpp"
#elif PLATFORM_MACOS
	#include "MacOS/MacOSMemory.hpp"
#else
	#error "Unsupported Platform!"
#endif

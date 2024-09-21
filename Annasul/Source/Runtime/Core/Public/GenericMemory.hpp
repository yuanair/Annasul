#pragma once

#include "Platform.hpp"
#include <cstring>

namespace Annasul
{
	
	struct FGenericMemory
	{
		
		FORCEINLINE static void *Copy(void *dest, const void *src, const SIZE_T size)
		{
			return ::memcpy(dest, src, size);
		}
		
		FORCEINLINE static void *Move(void *dest, const void *src, const SIZE_T size)
		{
			return ::memmove(dest, src, size);
		}
		
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
		
		
	};
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

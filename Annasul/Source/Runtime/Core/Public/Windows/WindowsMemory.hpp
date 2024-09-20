#pragma once

#include "GenericMemory.hpp"

namespace Annasul
{
	
	
	class FWindowsMemory : public FGenericMemory
	{
	public:
		
		static void* Malloc(SIZE_T count, uint32 alignment);
		static void* Realloc(void* original, SIZE_T count, uint32 alignment);
		static void Free(void* original);
		static SIZE_T GetAllocSize(void* original);
		
	};
	
	typedef FWindowsMemory FMemory;
	
}


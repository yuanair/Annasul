#include "Windows/WindowsMemory.hpp"

#include <cstdlib>

namespace Annasul
{
	
	void *FWindowsMemory::Malloc(const SIZE_T count, const uint32 alignment)
	{
		return ::_aligned_malloc(count, alignment);
	}
	
	void *FWindowsMemory::Realloc(void *original, const SIZE_T count, const uint32 alignment)
	{
		return ::_aligned_realloc(original, count, alignment);
	}
	
	void FWindowsMemory::Free(void *original)
	{
		::_aligned_free(original);
	}
	
	SIZE_T FWindowsMemory::GetAllocSize(void *original)
	{
		return ::_msize(original);
	}
	
}


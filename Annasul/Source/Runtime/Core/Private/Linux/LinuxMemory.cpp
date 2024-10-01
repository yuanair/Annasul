#include "Linux/LinuxMemory.hpp"

#include <cstdlib>

namespace Annasul
{
    void* FLinuxMemory::Malloc(const SIZE_T count, const uint32 alignment)
    {
        return ::std::aligned_alloc(alignment, count);
    }

    void* FLinuxMemory::Realloc(void* original, const SIZE_T count, uint32 alignment)
    {
        return ::std::realloc(original, count);
    }

    void FLinuxMemory::Free(void* original)
    {
        ::std::free(original);
    }

}


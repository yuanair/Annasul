#pragma once

#include "GenericMemory.hpp"

namespace Annasul
{

    struct FLinuxMemory : FGenericMemory
    {
        static void* Malloc(SIZE_T count, uint32 alignment);
        static void* Realloc(void* original, SIZE_T count, uint32 alignment);
        static void Free(void* original);
    };

    typedef FLinuxMemory FMemory;

}

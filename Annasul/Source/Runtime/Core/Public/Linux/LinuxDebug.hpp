#pragma once

#include "GenericDebug.hpp"

namespace Annasul
{

    class FLinuxDebug : public FGenericDebug
    {
    public:

        static FLinuxDebug& Get();

    public:

        void Log(EDebugLevel level, FStringView message, SourceLocation location = SourceLocation::current()) override;

    };

    typedef FLinuxDebug FDebug;

}

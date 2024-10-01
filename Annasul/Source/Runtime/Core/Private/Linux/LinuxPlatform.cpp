#include "Linux/LinuxPlatform.hpp"

#include <unistd.h>

namespace Annasul
{
    bool FLinuxPlatform::IsDarkMode()
    {
        return false;
    }

    bool FLinuxPlatform::IsLightMode()
    {
        return false;
    }

    bool FLinuxPlatform::IsUserAnAdmin()
    {
        return getuid() == 0;
    }
}



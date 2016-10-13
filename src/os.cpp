// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#include "error.hpp"
#include "os.hpp"

#include <Windows.h>

namespace os
{
    OSVERSIONINFOW get_version_info()
    {
        OSVERSIONINFOW info;
        ZeroMemory(&info, sizeof(info));
        info.dwOSVersionInfoSize = sizeof(info);

        if (!GetVersionExW(&info))
            error::raise("GetVersionExW");

        return info;
    }
}

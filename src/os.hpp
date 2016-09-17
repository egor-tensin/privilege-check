// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege test" project.
// For details, see https://github.com/egor-tensin/privilege-test.
// Distributed under the MIT License.

#pragma once

#include "error.hpp"

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

    bool is_vista_or_later(const OSVERSIONINFOW& info)
    {
        return info.dwMajorVersion >= 6;
    }

    bool is_vista_or_later()
    {
        return is_vista_or_later(get_version_info());
    }
}
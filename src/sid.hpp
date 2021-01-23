// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#pragma once

#include <windows.h>
#include <sddl.h>

#include <array>
#include <string>

constexpr DWORD max_sid_size = SECURITY_MAX_SID_SIZE;

typedef std::array<unsigned char, max_sid_size> SidBuffer;

namespace sid
{
    SidBuffer well_known(WELL_KNOWN_SID_TYPE type);

    SidBuffer builtin_administrators();

    std::wstring to_string(const SidBuffer&);
}

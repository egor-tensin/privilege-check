// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#pragma once

#include <Windows.h>

#include <string>

namespace resource
{
    HMODULE load_exe_module();

    std::wstring load_string(unsigned int id);
}

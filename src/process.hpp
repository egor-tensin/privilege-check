// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#pragma once

#include "cmd_line.hpp"

#include <Windows.h>

#include <string>

namespace process
{
    std::wstring get_executable_path();

    inline std::wstring get_command_line()
    {
        return GetCommandLine();
    }

    void runas(
        const CommandLine& cmd_line,
        HWND hwnd = NULL,
        int nShow = SW_NORMAL);

    void runas_self(
        HWND hwnd = NULL,
        int nShow = SW_NORMAL);
}

// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#pragma once

#include "cmd_line.hpp"
#include "error.hpp"

#include <Windows.h>
#include <shellapi.h>

#include <array>
#include <string>

namespace process
{
    std::wstring get_executable_path()
    {
        static constexpr DWORD max_path = MAX_PATH;

        std::array<wchar_t, max_path> buf;

        const auto ret = GetModuleFileNameW(NULL, buf.data(), max_path);

        if (GetLastError() != ERROR_SUCCESS)
            error::raise("GetModuleFileNameW");

        return buf.data();
    }

    std::wstring get_command_line()
    {
        return GetCommandLine();
    }

    void runas(
        const CommandLine& cmd_line,
        HWND hwnd = NULL,
        int nShow = SW_NORMAL)
    {
        static constexpr auto sep = L' ';

        const auto exe_path = cmd_line.has_argv0()
            ? cmd_line.get_argv0()
            : get_executable_path();

        SHELLEXECUTEINFOW info;
        ZeroMemory(&info, sizeof(info));
        info.cbSize = sizeof(info);
        info.lpVerb = L"runas";
        info.lpFile = exe_path.c_str();
        const auto args = cmd_line.join_args();
        if (!args.empty())
            info.lpParameters = args.c_str();
        info.hwnd = hwnd;
        info.nShow = nShow;

        if (!ShellExecuteExW(&info))
            error::raise("ShellExecuteExW");
    }

    void runas_self(
        HWND hwnd = NULL,
        int nShow = SW_NORMAL)
    {
        runas(CommandLine::query(), hwnd, nShow);
    }
}

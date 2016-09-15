#pragma once

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

    void runas(const std::wstring& exe_path, HWND hwnd = NULL, int nShow = SW_NORMAL)
    {
        SHELLEXECUTEINFOW info;
        ZeroMemory(&info, sizeof(info));
        info.cbSize = sizeof(info);
        info.lpVerb = L"runas";
        info.lpFile = exe_path.c_str();
        info.hwnd = hwnd;
        info.nShow = nShow;

        if (!ShellExecuteExW(&info))
            error::raise("ShellExecuteExW");
    }
}

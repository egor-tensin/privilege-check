#pragma once

#include "error.hpp"

#include <cstddef>

#include <string>

namespace resource
{
    HMODULE load_exe_module()
    {
        const auto module = GetModuleHandle(NULL);
        if (module == NULL)
            error::raise("GetModuleHandle");
        return module;
    }

    std::wstring load_string(unsigned int id)
    {
        wchar_t* s = nullptr;

        const auto ret = LoadStringW(
            load_exe_module(), id, reinterpret_cast<wchar_t*>(&s), 0);

        if (ret <= 0)
            error::raise("LoadStringW");

        return {s, static_cast<std::size_t>(ret)};
    }
}

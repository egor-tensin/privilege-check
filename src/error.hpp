#pragma once

#include <Windows.h>

#include <system_error>

typedef std::system_error Error;

namespace error
{
    inline void raise(const char* function_name)
    {
        const auto ec = GetLastError();
        throw std::system_error(ec, std::system_category(), function_name);
    }

    void report(const Error& e)
    {
        MessageBoxA(NULL, e.what(), NULL, MB_OK);
    }

    int get_code(const Error& e)
    {
        return e.code().value();
    }
}

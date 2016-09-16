#pragma once

#include <Windows.h>

#include <system_error>

typedef std::system_error Error;

namespace error
{
    Error make(const char* function_name)
    {
        const auto ec = GetLastError();
        return {static_cast<int>(ec), std::system_category(), function_name};
    }

    inline void raise(const char* function_name)
    {
        throw make(function_name);
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

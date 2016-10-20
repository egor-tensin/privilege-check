// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#pragma once

#include <Windows.h>

#include <exception>
#include <system_error>

typedef std::system_error Error;

namespace error
{
    inline Error make(const char* function_name)
    {
        const auto ec = GetLastError();
        return {static_cast<int>(ec), std::system_category(), function_name};
    }

    inline void raise(const char* function_name)
    {
        throw make(function_name);
    }

    inline void report(const std::exception& e)
    {
        MessageBoxA(NULL, e.what(), NULL, MB_OK);
    }

    inline int get_code(const Error& e)
    {
        return e.code().value();
    }
}

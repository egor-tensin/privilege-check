// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#pragma once

#include "error.hpp"

#include <Windows.h>

#include <exception>
#include <system_error>

namespace error
{
    Error make(const char* function_name)
    {
        const auto ec = GetLastError();
        return {static_cast<int>(ec), std::system_category(), function_name};
    }

    void raise(const char* function_name)
    {
        throw make(function_name);
    }

    void report(const std::exception& e)
    {
        MessageBoxA(NULL, e.what(), NULL, MB_OK);
    }

    int get_code(const Error& e)
    {
        return e.code().value();
    }
}

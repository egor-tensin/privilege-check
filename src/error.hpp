// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#pragma once

#include <exception>
#include <system_error>

typedef std::system_error Error;

namespace error
{
    Error make(const char* function_name);

    void raise(const char* function_name);

    void report(const std::exception&);

    int get_code(const Error&);
}

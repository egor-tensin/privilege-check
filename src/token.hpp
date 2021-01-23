// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#pragma once

#include "handle.hpp"
#include "sid.hpp"

#include <windows.h>

#include <string>

namespace token
{
    namespace permissions
    {
        constexpr DWORD query() { return TOKEN_QUERY; }

        constexpr DWORD duplicate() { return TOKEN_DUPLICATE; }
    }

    constexpr DWORD default_permissions = permissions::query();

    // Current thread's primary impersonation token.
    inline Handle dumb() { return Handle{NULL}; }

    Handle open_for_process(
        const Handle& process,
        DWORD permissions = default_permissions);

    Handle open_for_current_process(
        DWORD permissions = default_permissions);

    bool get_linked(Handle& token);

    Handle get_for_identification(const Handle& token);

    DWORD query_integrity_level(const Handle& token);

    std::wstring integrity_level_to_string(DWORD);

    bool check_belongs(const Handle& token, const SidBuffer& sid);

    bool query_elevation(const Handle& token);
}

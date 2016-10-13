// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#pragma once

#include "error.hpp"
#include "handle.hpp"
#include "os.hpp"
#include "sid.hpp"

#include <Windows.h>

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace token
{
    namespace permissions
    {
        constexpr DWORD query() { return TOKEN_QUERY; }

        constexpr DWORD duplicate() { return TOKEN_DUPLICATE; }
    }

    constexpr DWORD default_permissions = permissions::query();

    // Current thread's primary impersonation token.
    Handle dumb() { return Handle{NULL}; }

    Handle open_for_process(
        const Handle& process,
        DWORD permissions = default_permissions)
    {
        HANDLE raw;

        if (!OpenProcessToken(process, permissions, &raw))
            error::raise("OpenProcessToken");

        return Handle{raw};
    }

    Handle open_for_current_process(
        DWORD permissions = default_permissions)
    {
        return open_for_process(Handle{GetCurrentProcess()}, permissions);
    }

    bool get_linked(Handle& token)
    {
        if (!os::is_vista_or_later())
            return false;

        auto type = TokenElevationTypeDefault;
        DWORD cb = 0;

        if (!GetTokenInformation(token, TokenElevationType, &type, sizeof(type), &cb))
            error::raise("GetTokenInformation");

        if (type != TokenElevationTypeLimited)
            return false;

        HANDLE raw;

        if (!GetTokenInformation(token, TokenLinkedToken, &raw, sizeof(raw), &cb))
            error::raise("GetTokenInformation");

        token = Handle{raw};
        return true;
    }

    Handle get_for_identification(const Handle& token)
    {
        HANDLE raw;

        if (!DuplicateToken(token, SecurityIdentification, &raw))
            error::raise("DuplicateToken");

        return Handle{raw};
    }

    //
    // FUNCTION: query_integrity_level()
    //
    // RETURN VALUE: Returns the integrity level of the current process. It is
    // usually one of these values:
    //
    //   SECURITY_MANDATORY_UNTRUSTED_RID (SID: S-1-16-0x0)
    //   Means untrusted level. It is used by processes started by the
    //   Anonymous group. Blocks most write access.
    //
    //   SECURITY_MANDATORY_LOW_RID (SID: S-1-16-0x1000)
    //   Means low integrity level. It is used by Protected Mode Internet
    //   Explorer. Blocks write acess to most objects (such as files and
    //   registry keys) on the system.
    //
    //   SECURITY_MANDATORY_MEDIUM_RID (SID: S-1-16-0x2000)
    //   Means medium integrity level. It is used by normal applications
    //   being launched while UAC is enabled.
    //
    //   SECURITY_MANDATORY_HIGH_RID (SID: S-1-16-0x3000)
    //   Means high integrity level. It is used by administrative applications
    //   launched through elevation when UAC is enabled, or normal
    //   applications if UAC is disabled and the user is an administrator.
    //
    //   SECURITY_MANDATORY_SYSTEM_RID (SID: S-1-16-0x4000)
    //   Means system integrity level. It is used by services and other
    //   system-level applications (such as Wininit, Winlogon, Smss, etc.)
    //
    DWORD query_integrity_level(const Handle& token)
    {
        DWORD cb = 0;

        if (!GetTokenInformation(token, TokenIntegrityLevel, NULL, 0, &cb))
        {
            switch (GetLastError())
            {
                case ERROR_INSUFFICIENT_BUFFER:
                    break;
                default:
                    error::raise("GetTokenInformation");
            }
        }

        std::vector<unsigned char> buf(cb);
        const auto token_level = reinterpret_cast<TOKEN_MANDATORY_LABEL*>(buf.data());

        if (!GetTokenInformation(token, TokenIntegrityLevel, token_level, cb, &cb))
            error::raise("GetTokenInformation");

        // Integrity Level SIDs are in the form of S-1-16-0xXXXX. (e.g.
        // S-1-16-0x1000 stands for low integrity level SID). There is one and
        // only one subauthority.
        return *GetSidSubAuthority(token_level->Label.Sid, 0);
    }

    std::wstring integrity_level_to_string(DWORD level)
    {
        static const std::unordered_map<DWORD, const wchar_t*> names =
        {
            {SECURITY_MANDATORY_UNTRUSTED_RID, L"Untrusted"},
            {SECURITY_MANDATORY_LOW_RID, L"Low"},
            {SECURITY_MANDATORY_MEDIUM_RID, L"Medium"},
            {SECURITY_MANDATORY_HIGH_RID, L"High"},
            {SECURITY_MANDATORY_SYSTEM_RID, L"System"},
        };

        static constexpr auto unknown_name = L"Unknown";

        const auto it = names.find(level);

        if (it == names.cend())
            return unknown_name;

        return it->second;
    }

    bool check_belongs(const Handle& token, const SidBuffer& sid)
    {
        BOOL b = FALSE;

        if (!CheckTokenMembership(token, const_cast<unsigned char*>(sid.data()), &b))
            error::raise("CheckTokenMembership");

        return b != FALSE;
    }

    bool query_elevation(const Handle& token)
    {
        TOKEN_ELEVATION elevation;
        DWORD cb = 0;

        if (!GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &cb))
            error::raise("GetTokenInformation");

        return elevation.TokenIsElevated != 0;
    }
}

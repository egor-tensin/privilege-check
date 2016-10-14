// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#include "error.hpp"
#include "sid.hpp"

#include <Windows.h>
#include <sddl.h>

#include <memory>
#include <string>

namespace sid
{
    SidBuffer well_known(WELL_KNOWN_SID_TYPE type)
    {
        SidBuffer buffer;
        DWORD cb = static_cast<DWORD>(buffer.size());

        if (!CreateWellKnownSid(type, NULL, buffer.data(), &cb))
            error::raise("CreateWellKnownSid");

        return buffer;
    }

    SidBuffer builtin_administrators()
    {
        /*
        void* sid = nullptr;
        SID_IDENTIFIER_AUTHORITY authority = SECURITY_NT_AUTHORITY;

        if (!AllocateAndInitializeSid(
            &authority,
            2,
            SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0,
            &sid))
        {
            error::raise("AllocateAndInitializeSid");
        }

        return std::unique_ptr<void, FreeSid>{sid};
        */

        return well_known(WinBuiltinAdministratorsSid);
    }

    namespace
    {
        struct LocalDelete
        {
            void operator()(wchar_t* s) const
            {
                LocalFree(s);
            }
        };
    }

    std::wstring to_string(const SidBuffer& sid)
    {
        wchar_t* s = nullptr;

        if (!ConvertSidToStringSidW(const_cast<unsigned char*>(sid.data()), &s))
            error::raise("ConvertSidToStringSidW");

        return std::unique_ptr<wchar_t, LocalDelete>{s}.get();
    }
}

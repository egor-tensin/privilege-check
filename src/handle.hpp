// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#pragma once

#include <windows.h>

#include <cassert>

#include <memory>
#include <utility>

class Handle
{
public:
    Handle() = default;

    explicit Handle(HANDLE raw)
        : impl{raw}
    { }

    Handle(Handle&& other) noexcept
    {
        swap(other);
    }

    Handle& operator=(Handle other) noexcept
    {
        swap(other);
        return *this;
    }

    void swap(Handle& other) noexcept
    {
        using std::swap;
        swap(impl, other.impl);
    }

    operator HANDLE() const
    {
        return impl.get();
    }

private:
    struct Close
    {
        void operator()(HANDLE raw) const
        {
            if (raw == NULL || raw == INVALID_HANDLE_VALUE)
                return;
            const auto ret = CloseHandle(raw);
            assert(ret);
            (void) ret;
        }
    };

    std::unique_ptr<void, Close> impl;

    Handle(const Handle&) = delete;
};

inline void swap(Handle& a, Handle& b) noexcept
{
    a.swap(b);
}

namespace std
{
    template <>
    inline void swap(Handle& a, Handle& b) noexcept
    {
        a.swap(b);
    }
}

// Copyright (c) 2016 Egor Tensin <Egor.Tensin@gmail.com>
// This file is part of the "Privilege check" project.
// For details, see https://github.com/egor-tensin/privilege-check.
// Distributed under the MIT License.

#pragma once

#include <cctype>
#include <cstddef>

#include <algorithm>
#include <locale>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace string
{
    template <typename Char>
    void ltrim(std::basic_string<Char>& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [] (const Char& c)
        {
            return !std::isspace(c);
        }));
    }

    template <typename Char>
    void rtrim(std::basic_string<Char>& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [] (const Char& c)
        {
            return !std::isspace(c);
        }).base(), s.end());
    }

    template <typename Char>
    void trim(std::basic_string<Char>& s)
    {
        ltrim(s);
        rtrim(s);
    }

    template <typename Char, typename Sep, typename InputIterator>
    std::basic_string<Char> join(
        const Sep& sep,
        InputIterator beg,
        InputIterator end)
    {
        std::basic_ostringstream<Char> oss;

        if (beg != end)
        {
            oss << *beg;
            ++beg;
        }

        for (; beg != end; ++beg)
            oss << sep << *beg;

        return oss.str();
    }

    template <typename Char, typename Sep>
    std::basic_string<Char> join(
        const Sep& sep,
        const std::vector<std::basic_string<Char>>& args)
    {
        return join<Char>(sep, args.cbegin(), args.cend());
    }

    template <typename Char, typename String, typename = void>
    struct StringHelper
    { };

    template <typename Char, typename String>
    struct StringHelper<Char, String, typename std::enable_if<std::is_same<typename std::decay<typename std::remove_pointer<String>::type>::type, Char>::value>::type>
    {
        inline StringHelper(const Char& c)
            : buf{&c}
            , len{1}
        { }

        inline StringHelper(const Char* s)
            : buf{s}
            , len{std::char_traits<Char>::length(s)}
        { }

        inline const Char* buffer() const { return buf; }

        inline std::size_t length() const { return len; }

    private:
        const Char* const buf;
        const std::size_t len;
    };

    template <typename Char, typename String>
    struct StringHelper<Char, String, typename std::enable_if<std::is_same<String, std::basic_string<Char>>::value>::type>
    {
        inline StringHelper(const std::basic_string<Char>& s)
            : s{s}
        { }

        inline const Char* buffer() const { return s.c_str(); }

        inline std::size_t length() const { return s.length(); }

    private:
        const std::basic_string<Char>& s;
    };

    template <typename Char, typename What, typename By>
    void replace(
        std::basic_string<Char>& s,
        const What& what,
        const By& by)
    {
        std::size_t pos = 0;

        const StringHelper<Char, typename std::decay<What>::type> what_helper{what};
        const StringHelper<Char, typename std::decay<By>::type> by_helper{by};

        const auto what_buf = what_helper.buffer();
        const auto what_len = what_helper.length();

        const auto by_buf = by_helper.buffer();
        const auto by_len = by_helper.length();

        while ((pos = s.find(what_buf, pos, what_len)) != std::basic_string<Char>::npos)
        {
            s.replace(pos, what_len, by_buf, by_len);
            pos += by_len;
        }
    }

    template <typename Char, typename What>
    void prefix_with(
        std::basic_string<Char>& s,
        const What& what,
        const Char& by)
    {
        const StringHelper<Char, typename std::decay<What>::type> what_helper{what};

        const auto what_buf = what_helper.buffer();
        const auto what_len = what_helper.length();

        std::size_t numof_by = 0;

        for (std::size_t i = 0; i < what_len; ++i)
            numof_by += std::count(s.cbegin(), s.cend(), what_buf[i]);

        s.reserve(s.capacity() + numof_by);

        for (std::size_t i = 0; i < what_len; ++i)
            replace(s, what_buf[i], by);
    }
}

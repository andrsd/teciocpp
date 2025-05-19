// SPDX-FileCopyrightText: 2025 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <vector>
#include <numeric>

namespace teciocpp {
namespace utils {

template <typename T>
inline std::string
join(const std::string & delimiter, const std::vector<T> & words)
{
    if (words.empty())
        return "";
    return std::accumulate(words.begin() + 1,
                           words.end(),
                           std::to_string(words[0]),
                           [&delimiter](const std::string & a, const T & b) {
                               return a + delimiter + std::to_string(b);
                           });
}

template <>
inline std::string
join(const std::string & delimiter, const std::vector<std::string> & words)
{
    if (words.empty())
        return "";
    return std::accumulate(
        words.begin() + 1,
        words.end(),
        words[0],
        [&delimiter](const std::string & a, const std::string & b) { return a + delimiter + b; });
}

} // namespace utils
} // namespace teciocpp

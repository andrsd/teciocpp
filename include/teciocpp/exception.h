// SPDX-FileCopyrightText: 2025 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <stdexcept>
#include <string>
#include "fmt/format.h"

namespace teciocpp {

class Exception : public std::exception {
public:
    template <typename... T>
    Exception(fmt::format_string<T...> format, T... args)
    {
        this->msg_ = fmt::format(format, std::forward<T>(args)...);
    }

    /// Get the exception message
    const char * what() const noexcept override;

private:
    /// Error message
    std::string msg_;
};

} // namespace teciocpp

// SPDX-FileCopyrightText: 2025 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include <stdexcept>
#include <string>

namespace teciocpp {

class Exception : public std::runtime_error {
public:
    explicit Exception(const std::string & what_arg);
    explicit Exception(const char * what_arg);

protected:
};

} // namespace teciocpp

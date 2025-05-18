// SPDX-FileCopyrightText: 2025 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "teciocpp/exception.h"

namespace teciocpp {

Exception::Exception(const std::string & what_arg) : std::runtime_error(what_arg) {}

Exception::Exception(const char * what_arg) : std::runtime_error(what_arg) {}

} // namespace teciocpp

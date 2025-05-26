// SPDX-FileCopyrightText: 2025 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#include "teciocpp/impl/exception.h"

namespace teciocpp {

const char *
Exception::what() const noexcept
{
    return this->msg_.c_str();
}

} // namespace teciocpp

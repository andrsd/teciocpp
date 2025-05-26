// SPDX-FileCopyrightText: 2025 David Andrs <andrsd@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

namespace teciocpp {

enum class ZoneType {
    ORDERED,
    FELINE,
    FETRIANGLE,
    FEQUADRILATERAL,
    FETETRAHEDRON,
    FEBRICK,
    FEPOLYGON,
    FEPOLYHEDRON
};

enum class ValueLocation {
    ///
    NODAL,
    ELEMENTAL
};

} // namespace teciocpp

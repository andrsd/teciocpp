# Find TecIO
#
# Once done this will define
#  TECIO_FOUND - System has TecIO
#  TECIO_INCLUDE_DIR - The TecIO include directory
#  TECIO_LIBRARY - The TecIO library

if (NOT DEFINED ENV{TECPLOT_DIR})
    message(FATAL_ERROR "Environment variable TECPLOT_DIR not set")
endif()

find_path(
    TECIO_INCLUDE_DIR
        TECIO.h
    PATHS
        $ENV{TECPLOT_DIR}/include
)

if (APPLE)
    find_library(
        TECIO_LIBRARY
            tecio
        PATHS
            $ENV{TECPLOT_DIR}/Frameworks
    )
else()
    find_library(
        TECIO_LIBRARY
            tecio
        PATHS
            $ENV{TECPLOT_DIR}/lib
    )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    TecIO
    REQUIRED_VARS
        TECIO_LIBRARY
        TECIO_INCLUDE_DIR
)

if (TECIO_FOUND AND NOT TARGET tecio::tecio)
    add_library(tecio::tecio UNKNOWN IMPORTED)
    set_target_properties(tecio::tecio
        PROPERTIES
            IMPORTED_LOCATION "${TECIO_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${TECIO_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(FORCE
    TECIO_INCLUDE_DIR
    TECIO_LIBRARY
)

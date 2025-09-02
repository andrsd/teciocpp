cmake_minimum_required(VERSION 3.16)

project(teciocpp
    VERSION 0.2.0
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(CodeCoverage)

option(TECIOCPP_WITH_MPI "Build with MPI support" YES)
option(TECIOCPP_BUILD_TESTS "Build tests" NO)
option(TECIOCPP_BUILD_TOOLS "Build tools" YES)

find_package(Boost REQUIRED CONFIG)
if (Boost_FOUND)
    message(STATUS "Found Boost: ${Boost_INCLUDE_DIRS} (found version \"${Boost_VERSION}\")")
endif()
find_package(fmt 11 REQUIRED)
if (TECIOCPP_WITH_MPI)
    find_package(MPI REQUIRED)
endif()

add_subdirectory(contrib/tecio)
add_subdirectory(src)

# Tests

if (TECIOCPP_BUILD_TESTS)
    set(TECIOCPP_UNIT_TEST_ASSETS ${CMAKE_SOURCE_DIR}/test/assets CACHE PATH "" FORCE)
    mark_as_advanced(FORCE TECIOCPP_UNIT_TEST_ASSETS)

    enable_testing()
    add_subdirectory(test)
endif()

cmake_minimum_required(VERSION 3.10)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE Release CACHE STRING "CMake build type (Debug|Release)" FORCE)
endif ()

project (teciompi)

FIND_PACKAGE(Boost 1.69.0 CONFIG)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ELSE()
    Set (Boost_INCLUDE_DIR /3rdpartylibs/boost/1.69.0/build/linux64-centos6.10/release/include)
    message ("Warning:  Cannot find boost include directories.  Hardcoding to ${Boost_INCLUDE_DIR}")
    INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR})
ENDIF()

FIND_PACKAGE(MPI)
IF (NOT MPI_CXX_FOUND)
    message(FATAL_ERROR "Unable to find MPI installation.")
ENDIF ()

include_directories(.)
if(NOT "${MPI_CXX_INCLUDE_PATH}" STREQUAL "")
    include_directories(SYSTEM "${MPI_CXX_INCLUDE_PATH}")
endif()

set (CMAKE_CXX_STANDARD 14)

IF (WIN32)
    set(BaseFlags "/EHsc /MP /wd\"4996\" /D MSWIN /D TP_PROJECT_USES_BOOST /D BOOST_ALL_NO_LIB /D MAKEARCHIVE /D NO_THIRD_PARTY_LIBS /D TECIOMPI /D NO_ASSERTS /D tecio_EXPORTS")
ELSE ()
    set(BaseFlags "-DLINUX -DLINUX64 -DTP_PROJECT_USES_BOOST -DBOOST_ALL_NO_LIB -DMAKEARCHIVE -DNO_THIRD_PARTY_LIBS -DTECIOMPI -DOMPI_SKIP_MPICXX -DMPICH_SKIP_MPICXX -DMPI_NO_CPPBIND -DNO_ASSERTS -Dtecio_EXPORTS -fmessage-length=0 -fPIC -fvisibility=hidden -w")
ENDIF ()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${BaseFlags} ${MPI_CXX_COMPILE_FLAGS}")

FILE(GLOB mainFiles "*.cpp" "*.h")
LIST(REMOVE_ITEM mainFiles "${CMAKE_CURRENT_SOURCE_DIR}/szcombine.cpp")

add_library(teciompi STATIC ${mainFiles})

add_executable(szcombine "szcombine.cpp")
target_link_libraries(szcombine teciompi ${MPI_LIBRARIES})

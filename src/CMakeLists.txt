file(GLOB_RECURSE SRCS *.cpp)

add_library(${PROJECT_NAME} SHARED ${SRCS})

if (CMAKE_PROJECT_NAME STREQUAL "teciocpp")
    target_code_coverage(${PROJECT_NAME})
endif()

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)

if (TECIOCPP_WITH_MPI)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TECIOCPP_WITH_MPI TECIOMPI)
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/contrib/tecio/teciompisrc>
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE teciompi MPI::MPI_C)
else()
    target_include_directories(${PROJECT_NAME}
        PRIVATE
            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/contrib/tecio/teciosrc>
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE tecio)
endif()


# Install

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/cmake/teciocpp-config.cmake.in
    teciocpp-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/teciocpp
    NO_SET_AND_CHECK_MACRO
)
write_basic_package_version_file(teciocpp-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    TARGETS teciocpp
    EXPORT teciocpp-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

install(
    EXPORT teciocpp-targets
    FILE teciocpp-targets.cmake
    NAMESPACE teciocpp::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/teciocpp
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/teciocpp-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/teciocpp-config-version.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/teciocpp
)

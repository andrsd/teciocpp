project(teciocpp-test)

include(GoogleTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRCS *.cpp)

add_executable(${PROJECT_NAME} ${SRCS})
target_code_coverage(${PROJECT_NAME})
target_sanitization(${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        teciocpp
        gtest_main
        gmock_main
)

if (TECIOCPP_WITH_MPI)
    target_link_libraries(${PROJECT_NAME} PRIVATE teciompi MPI::MPI_C)
endif()

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        -DTECIOCPP_UNIT_TEST_ASSETS="${TECIOCPP_UNIT_TEST_ASSETS}"
)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)
if (TECIOCPP_CODE_COVERAGE)
    set_tests_properties(
        ${PROJECT_NAME}
        PROPERTIES
            ENVIRONMENT LLVM_PROFILE_FILE=${PROJECT_NAME}.profraw
    )
endif()

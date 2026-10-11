cmake_minimum_required(VERSION 3.20)

project(seed_fund LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(seedlot
    src/seedlot.cpp
)

target_include_directories(seedlot
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(seedlot
    PRIVATE -Wall -Wextra -Wpedantic
)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

add_executable(seedlot_tests
    tests/seedlot_test.cpp
)

target_link_libraries(seedlot_tests
    PRIVATE
        seedlot
        GTest::gtest_main
)

include(CTest)
include(GoogleTest)

gtest_discover_tests(seedlot_tests)
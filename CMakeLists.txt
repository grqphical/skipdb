cmake_minimum_required(VERSION 3.20)
project(SkipDB
    VERSION 0.1.0
    DESCRIPTION "An embeddable key-value database, centered around the use of a Skip List"
    LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build shared libraries instead of static" OFF)

add_library(skipdb)

target_sources(skipdb
    PRIVATE
        src/skipdb.c
        src/skip_list.c
        src/arena_allocator.c
)

target_include_directories(skipdb
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(skipdb PRIVATE -Wall -Wextra -Wpedantic)

set_target_properties(skipdb PROPERTIES
    C_VISIBILITY_PRESET hidden    # Hide internal functions by default (shared builds)
    VISIBILITY_INLINES_HIDDEN ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
    )
    # For Windows: prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(GLOB_RECURSE SKIPDB_TEST_SOURCES CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cc
    )

    add_executable(skipdb_tests ${SKIPDB_TEST_SOURCES})
    target_link_libraries(skipdb_tests
        PRIVATE
            skipdb
            GTest::gtest
            GTest::gtest_main
    )
    target_include_directories(skipdb_tests
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

    include(GoogleTest)
    gtest_discover_tests(skipdb_tests)
endif()

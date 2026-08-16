cmake_minimum_required(VERSION 3.28)
project(SkipDB
    VERSION 0.1.0
    DESCRIPTION "An embeddable key-value database, centered around the use of a Skip List"
    HOMEPAGE_URL "https://github.com/grqphical/skipdb"
    LANGUAGES C CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(SKIPDB_IS_TOP_LEVEL ON)
else()
    set(SKIPDB_IS_TOP_LEVEL OFF)
endif()

option(BUILD_TESTS "Build unit tests" ${SKIPDB_IS_TOP_LEVEL})
option(BUILD_CLI "Builds the SkipDB CLI" ${SKIPDB_IS_TOP_LEVEL})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build shared libraries instead of static" OFF)

add_library(skipdb)
add_library(SkipDB::skipdb ALIAS skipdb)

target_sources(skipdb
    PRIVATE
        src/skipdb.c
        src/skip_list.c
        src/arena_allocator.c
)

include(GenerateExportHeader)
generate_export_header(skipdb
    BASE_NAME SKIPDB
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/skipdb/export.h
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/skipdb/export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/skipdb)

target_include_directories(skipdb
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(skipdb PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(skipdb PRIVATE /W4)
endif()

set_target_properties(skipdb PROPERTIES
    C_VISIBILITY_PRESET hidden    # Hide internal functions by default (shared builds)
    VISIBILITY_INLINES_HIDDEN ON
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        EXCLUDE_FROM_ALL
        SYSTEM
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

if (BUILD_CLI)
    add_executable(skipdb_cli
        cli/cli.c
        cli/parser.c
    )
    target_link_libraries(skipdb_cli
        PRIVATE
        skipdb
    )
    target_include_directories(skipdb_cli
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/cli
            ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
endif()


install(TARGETS skipdb
    EXPORT SkipDBTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT SkipDBTargets
    FILE SkipDBTargets.cmake
    NAMESPACE SkipDB::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SkipDB
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/SkipDBConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/SkipDBConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SkipDB
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/SkipDBConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/SkipDBConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/SkipDBConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SkipDB
)

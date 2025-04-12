cmake_minimum_required(VERSION 3.12)
project(SWIFTNETPP VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(SWIFTNETPP_BUILD_SHARED "Build as shared library" ON)
option(SWIFTNETPP_BUILD_TESTS "Build tests" OFF)

include_directories(include)

add_subdirectory(src)

if(SWIFTNETPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(
    DIRECTORY include/snpp
    DESTINATION include
    FILES_MATCHING PATTERN "*.h"
)

install(
    TARGETS swiftnetpp
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
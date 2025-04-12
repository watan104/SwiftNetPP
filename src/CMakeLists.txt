file(GLOB_RECURSE SOURCES 
    "ai/*.c"
    "net/*.c"
    "utils/*.c"
)

if(SWIFTNETPP_BUILD_SHARED)
    add_library(swiftnetpp SHARED ${SOURCES})
else()
    add_library(swiftnetpp STATIC ${SOURCES})
endif()

target_include_directories(swiftnetpp PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_compile_options(swiftnetpp PRIVATE 
    -Wall -Wextra -O3 -fPIC
)

if(WIN32)
    target_link_libraries(swiftnetpp PRIVATE ws2_32)
else()
    target_link_libraries(swiftnetpp PRIVATE pthread)
endif()
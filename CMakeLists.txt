cmake_minimum_required(VERSION 3.31)
project(dpkvs)

set(CMAKE_CXX_STANDARD 23)

enable_testing()

include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)

add_executable(dpkvs_server
        cmd/server/main.cpp
)

target_include_directories(dpkvs_server PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(dpkvs)
add_subdirectory(tests)

target_link_libraries(dpkvs_server PRIVATE
        dpkvs_master_lib
)

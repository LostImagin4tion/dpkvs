cmake_minimum_required(VERSION 3.31)
project(dpkvs)

set(CMAKE_CXX_STANDARD 23)

enable_testing()

include(cmake/dependencies.cmake)

add_executable(dpkvs_app
        main.cpp
)

target_include_directories(dpkvs_app PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(dpkvs)
add_subdirectory(tests)

target_link_libraries(dpkvs_app PRIVATE
        dpkvs_lib
)

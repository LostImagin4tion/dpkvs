enable_testing()

#set(CMAKE_CXX_FLAGS --coverage)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

add_executable(dpkvs_tests
        unit/engine/engine_basic_operations_test.cpp
        unit/engine/engine_instantiating_test.cpp
        unit/append_only_log/serializer_test.cpp
)

target_include_directories(dpkvs_tests PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(dpkvs_tests
        GTest::gtest_main
        dpkvs_lib
)

include(GoogleTest)
gtest_discover_tests(dpkvs_tests)

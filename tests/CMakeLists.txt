enable_testing()

include(${CMAKE_SOURCE_DIR}/cmake/test_dependencies.cmake)

add_executable(dpkvs_tests
        # unit tests
        unit/engine/engine_basic_operations_test.cpp
        unit/engine/engine_instantiating_test.cpp
        unit/append_only_log/serializer_test.cpp
        unit/append_only_log/append_only_log_test.cpp

        # integration tests
        integration/network/server/dpkvs_service_impl_test.cpp
)

target_include_directories(dpkvs_tests PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(dpkvs_tests
        GTest::gtest_main
        dpkvs_lib
)

set_tests_properties(
        PROPERTIES LABELS "dpkvs_tests"
)

include(GoogleTest)
gtest_discover_tests(dpkvs_tests)

add_custom_target(run_tests
        COMMAND ${CMAKE_CTEST_COMMAND} -L dpkvs_tests
        DEPENDS dpkvs_tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Run all tests"
)

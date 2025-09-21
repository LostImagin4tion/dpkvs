enable_testing()

include(${CMAKE_SOURCE_DIR}/cmake/test_dependencies.cmake)

add_executable(dpkvs_tests
        # unit tests
        unit/core/engines/hash_map/runtime/hash_map_store_basic_operations_test.cpp
        unit/core/engines/hash_map/runtime/hash_map_store_concurrency_test.cpp
        unit/core/store_record/serializer/serializer_test.cpp
        unit/core/engines/hash_map/persistence/append_only_log_test.cpp

        # integration tests
        integration/network/service/dpkvs_service_impl_test.cpp
)

target_include_directories(dpkvs_tests PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(dpkvs_tests
        GTest::gtest_main
        dpkvs_master_lib
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

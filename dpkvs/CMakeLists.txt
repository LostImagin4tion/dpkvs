add_library(dpkvs_lib STATIC
        engine/store_engine.cpp
        engine/store_engine.h
        engine/storable_value.h
        engine/storable_value.cpp
        append_only_log/operations.h
        append_only_log/append_only_log.h
        append_only_log/append_only_log.cpp
        append_only_log/log_serializer.h
        append_only_log/log_serializer.cpp
        store_controller/store_controller.h
        store_controller/store_controller.cpp
        network/generated/dpkvs_service.grpc.pb.cc
        network/generated/dpkvs_service.pb.cc
        network/generated/get_operation.pb.cc
        network/generated/put_operation.pb.cc
        network/generated/remove_operation.pb.cc
)

target_include_directories(dpkvs_lib PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(dpkvs_lib PRIVATE
        grpc++
        protobuf::libprotobuf
)

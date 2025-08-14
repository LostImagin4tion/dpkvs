add_library(dpkvs_lib STATIC
        # store engine
        engine/store_engine.h
        engine/store_engine.cpp
        engine/storable_value.h
        engine/storable_value.cpp

        # append only log
        append_only_log/operations.h
        append_only_log/append_only_log.h
        append_only_log/append_only_log.cpp
        append_only_log/log_serializer.h
        append_only_log/log_serializer.cpp

        # store controller
        store_controller/store_controller.h
        store_controller/store_controller.cpp

        # service protos
        network/generated/dpkvs_service.grpc.pb.cc
        network/generated/dpkvs_service.pb.cc
        network/generated/get_operation.pb.cc
        network/generated/put_operation.pb.cc
        network/generated/remove_operation.pb.cc

        # service impl
        network/server/dpkvs_service_impl.h
        network/server/dpkvs_service_impl.cpp
)

target_include_directories(dpkvs_lib PUBLIC
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(dpkvs_lib PUBLIC
        grpc++
        grpc++_reflection
        protobuf::libprotobuf
        absl::flags
        absl::flags_parse
)

add_library(dpkvs_lib STATIC
        # store engine
        core/store_record/store_record.h
        core/store_record/store_record.cpp
        core/engines/store_engine.h
        core/engines/operations.h

        # ==== Hash Map store engine ====
        core/engines/hash_map/impl/hash_map_store_engine.h
        core/engines/hash_map/impl/hash_map_store_engine.cpp

        # append only log
        core/engines/hash_map/append_only_log/append_only_log.h
        core/engines/hash_map/append_only_log/append_only_log.cpp
        core/engines/hash_map/append_only_log/log_serializer.h
        core/engines/hash_map/append_only_log/log_serializer.cpp

        # store controller
        core/engines/hash_map/controller/store_controller.h
        core/engines/hash_map/controller/store_controller.cpp

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

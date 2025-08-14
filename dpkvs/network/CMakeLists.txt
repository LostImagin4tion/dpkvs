add_library(dpkvs_network STATIC
        # service protos
        generated/dpkvs_service.grpc.pb.cc
        generated/dpkvs_service.pb.cc
        generated/get_operation.pb.cc
        generated/put_operation.pb.cc
        generated/remove_operation.pb.cc

        # service impl
        server/dpkvs_service_impl.h
        server/dpkvs_service_impl.cpp
)

target_include_directories(dpkvs_network PUBLIC
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(dpkvs_network
        PRIVATE
        dpkvs_core

        PUBLIC
        grpc++
        grpc++_reflection
        protobuf::libprotobuf
        absl::flags
        absl::flags_parse
)

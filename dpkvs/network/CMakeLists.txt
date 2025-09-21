add_library(dpkvs_network STATIC
        # service protos
        generated/dpkvs_service.grpc.pb.h
        generated/dpkvs_service.grpc.pb.cc
        generated/dpkvs_service.pb.h
        generated/dpkvs_service.pb.cc
        generated/get_operation.pb.h
        generated/get_operation.pb.cc
        generated/put_operation.pb.h
        generated/put_operation.pb.cc
        generated/remove_operation.pb.h
        generated/remove_operation.pb.cc

        # service impl
        service/dpkvs_service_impl.h
        service/dpkvs_service_impl.cpp
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
)

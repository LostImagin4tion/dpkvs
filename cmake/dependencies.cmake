include(FetchContent)

set(ABSL_ENABLE_INSTALL ON)

FetchContent_Declare(
        gRPC
        GIT_REPOSITORY https://github.com/grpc/grpc
        GIT_TAG        v1.74.0
)
FetchContent_MakeAvailable(gRPC)

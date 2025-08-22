include(FetchContent)

set(ABSL_ENABLE_INSTALL ON)

FetchContent_Declare(
        gRPC
        GIT_REPOSITORY https://github.com/grpc/grpc
        GIT_TAG        v1.74.0
)
FetchContent_MakeAvailable(gRPC)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG        v1.15.3
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp
        GIT_TAG        0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

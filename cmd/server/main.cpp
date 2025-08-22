#include <dpkvs/network/service/dpkvs_service_impl.h>

#include <dpkvs/core/config/config.h>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <spdlog/spdlog.h>

#include <iostream>

using NKVStore::NService::TDpkvsServiceImpl;
using NKVStore::NCore::NConfig::TConfig;

void RunServer(const std::string& host, int64_t port) {
    auto serverAddress = std::format("{}:{}", host, port);

    auto logger = std::make_shared<TConsoleLogger>();
    auto service = std::make_unique<TDpkvsServiceImpl>("prod-append-only-log.txt", logger);

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    ServerBuilder builder;
    builder.AddListeningPort(
        serverAddress,
        grpc::InsecureServerCredentials());
    builder.RegisterService(service.get());

    std::unique_ptr<Server> server(builder.BuildAndStart());

    logger->Info("Server running on {}", serverAddress);

    server->Wait();
}

int main() {
    spdlog::init_thread_pool(8192, 2);

    auto config = TConfig::FromFile("../config/config.yaml");

    const auto host = config.GetString("server.host", "127.0.0.1");
    const auto port = config.GetInt("server.port", 50051);

    RunServer(host, port);

    spdlog::shutdown();
    return 0;
}
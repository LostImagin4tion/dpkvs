#include <dpkvs/network/service/dpkvs_service_impl.h>

#include <dpkvs/core/configs/build/build_config.h>
#include <dpkvs/core/configs/feature//feature_config.h>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <spdlog/spdlog.h>

#include <iostream>

using NKVStore::NService::TDpkvsServiceImpl;
using NKVStore::NCore::NConfig::TBuildConfig;
using NKVStore::NCore::NConfig::TFeatureConfig;

void RunServer(
    std::shared_ptr<TConsoleLogger> logger,
    const std::string& host,
    int64_t port,
    std::string persistenceLogPath)
{
    auto serverAddress = std::format("{}:{}", host, port);

    auto service = std::make_unique<TDpkvsServiceImpl>(
        std::move(persistenceLogPath),
        logger);

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

int main()
{
    spdlog::init_thread_pool(8192, 2);

    auto logger = std::make_shared<TConsoleLogger>();

    auto buildConfig = TBuildConfig();
    auto featureConfigPath = buildConfig.GetString("feature_config_path");

    auto featureConfig = TFeatureConfig::FromFile(featureConfigPath);

    const auto host = buildConfig.GetString("server_host");
    const auto port = buildConfig.GetInt("server_port");
    const auto persistenceLogPath = buildConfig.GetString("persistence_log_path");

    RunServer(logger, host, port, persistenceLogPath);

    spdlog::shutdown();
    return 0;
}

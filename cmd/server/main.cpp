#include <dpkvs/network/service/dpkvs_service_impl.h>

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <spdlog/spdlog.h>

#include <iostream>

ABSL_FLAG(std::string, address, "localhost:50051", "Server address for the service");

using NKVStore::NService::TDpkvsServiceImpl;

void RunServer(const std::string& server_address) { // TODO DPKVS-14 move it to app-like class
    auto logger = std::make_shared<TConsoleLogger>();
    auto service = std::make_unique<TDpkvsServiceImpl>("prod-append-only-log.txt", logger);

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    ServerBuilder builder;
    builder.AddListeningPort(
        server_address,
        grpc::InsecureServerCredentials());
    builder.RegisterService(service.get());

    std::unique_ptr<Server> server(builder.BuildAndStart());

    std::cout << "Server running on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    spdlog::init_thread_pool(8192, 2);

    absl::ParseCommandLine(argc, argv);
    RunServer(absl::GetFlag(FLAGS_address));

    spdlog::shutdown();
    return 0;
}
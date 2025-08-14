#include <dpkvs/network/server/dpkvs_service_impl.h>

#include <absl/flags/flag.h>
#include <absl/flags/parse.h>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <iostream>

ABSL_FLAG(std::string, address, "localhost:50051", "Server address for the service");

using NKVStore::NService::TDpkvsServiceImpl;

void RunServer(const std::string& server_address) {
    TDpkvsServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();

    ServerBuilder builder;

    builder.AddListeningPort(
        server_address,
        grpc::InsecureServerCredentials());

    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());

    std::cout << "Server running on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    absl::ParseCommandLine(argc, argv);
    RunServer(absl::GetFlag(FLAGS_address));
    return 0;
}
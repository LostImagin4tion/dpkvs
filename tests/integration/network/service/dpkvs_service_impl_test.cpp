#include <gtest/gtest.h>

#include <dpkvs/core/engines/hash_map/engine/hash_map_store_engine.h>
#include <dpkvs/network/service/dpkvs_service_impl.h>

#include <grpcpp/grpcpp.h>

using grpc::Server;
using grpc::ServerBuilder;

using NKVStore::NService::TDpkvsService;
using NKVStore::NService::TDpkvsServiceImpl;
using NKVStore::NCore::NEngine::THashMapStoreEngine;

class ServiceIntegrationTest
    : public testing::Test {

protected:
    void SetUp() override {
        spdlog::init_thread_pool(8192, 2);

        std::string serverAddress = "127.0.0.1:0";

        auto logger = std::make_shared<TConsoleLogger>();
        auto service = std::make_unique<TDpkvsServiceImpl>(_fileName, logger);

        grpc::EnableDefaultHealthCheckService(true);
        
        ServerBuilder builder;
        builder.AddListeningPort(
            serverAddress, 
            grpc::InsecureServerCredentials(), 
            &_selectedPort);
        builder.RegisterService(service.get());
        
        _service = std::move(service);
        _server = builder.BuildAndStart();

        ASSERT_TRUE(_server);
        ASSERT_NE(_selectedPort, 0);

        auto channel = grpc::CreateChannel(
            "127.0.0.1:" + std::to_string(_selectedPort),
            grpc::InsecureChannelCredentials());
        
        stub = TDpkvsService::NewStub(channel);
    }

    void TearDown() override {
        if (_server) {
            _server->Shutdown();
            _server->Wait();
            _server.reset();
        }
        _service.reset();

        std::filesystem::remove(_fileName);
    }

    std::unique_ptr<TDpkvsService::Stub> stub;

private:
    int _selectedPort = 0;
    std::string _fileName = "test_service_append_only_log.txt";

    std::unique_ptr<Server> _server;
    std::unique_ptr<TDpkvsServiceImpl> _service;
};

TEST_F(ServiceIntegrationTest, PutGetRemoveRoundtrip) {
    // === Put ===
    grpc::ClientContext putContext;
    NKVStore::NService::TPutRequest putRequest;
    putRequest.set_key("a");
    putRequest.set_value("1");
    
    NKVStore::NService::TPutResponse putResponse;
    auto status1 = stub->Put(
        &putContext, 
        putRequest, 
        &putResponse);
    
    ASSERT_TRUE(status1.ok());

    // === Get existing ===

    grpc::ClientContext getContext;
    NKVStore::NService::TGetRequest getRequest;
    getRequest.set_key("a");
    
    NKVStore::NService::TGetResponse getResponse;
    auto status2 = stub->Get(
        &getContext, 
        getRequest, 
        &getResponse);
    
    ASSERT_TRUE(status2.ok());
    ASSERT_EQ(getResponse.value(), "1");

    // === Remove ===

    grpc::ClientContext removeContext;
    NKVStore::NService::TRemoveRequest removeRequest;
    removeRequest.set_key("a");
    NKVStore::NService::TRemoveResponse removeResponse;
    
    auto status3 = stub->Remove(
        &removeContext,
        removeRequest, 
        &removeResponse);
    
    ASSERT_TRUE(status3.ok());
    ASSERT_TRUE(removeResponse.removed());

    // === Get missing ===

    grpc::ClientContext getContext2;
    NKVStore::NService::TGetRequest getRequest2;
    getRequest2.set_key("a");
    
    NKVStore::NService::TGetResponse getResponse2;
    auto status4 = stub->Get(
        &getContext2, 
        getRequest2, 
        &getResponse2);
    
    ASSERT_FALSE(status4.ok());
    ASSERT_EQ(status4.error_code(), grpc::StatusCode::NOT_FOUND);
}

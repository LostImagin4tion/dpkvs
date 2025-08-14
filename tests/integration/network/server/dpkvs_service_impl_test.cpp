#include <gtest/gtest.h>

#include <dpkvs/network/generated/dpkvs_service.grpc.pb.h>
#include <dpkvs/network/server/dpkvs_service_impl.h>

#include <grpcpp/grpcpp.h>

using grpc::Server;
using grpc::ServerBuilder;

using NKVStore::NService::TDpkvsService;
using NKVStore::NService::TDpkvsServiceImpl;
using NKVStore::NController::TStoreController;

class ServiceIntegrationTest
    : public ::testing::Test {

protected:
    void SetUp() override {
        std::string serverAddress = "127.0.0.1:0"; // ephemeral port

        auto controller = std::make_unique<TStoreController>("test_service_append_only_log.txt");
        auto service = std::make_unique<TDpkvsServiceImpl>(std::move(controller));

        grpc::EnableDefaultHealthCheckService(true);
        
        ServerBuilder builder;
        builder.AddListeningPort(
            serverAddress, 
            grpc::InsecureServerCredentials(), 
            &selected_port_);
        builder.RegisterService(service.get());
        
        _service = std::move(service);
        server_ = builder.BuildAndStart();

        ASSERT_TRUE(server_);
        ASSERT_NE(selected_port_, 0);

        auto channel = grpc::CreateChannel(
            "127.0.0.1:" + std::to_string(selected_port_),
            grpc::InsecureChannelCredentials());
        
        _stub = TDpkvsService::NewStub(channel);
    }

    void TearDown() override {
        if (server_) {
            server_->Shutdown();
            server_->Wait();
            server_.reset();
        }
        _service.reset();
    }

    std::unique_ptr<Server> server_;
    std::unique_ptr<TDpkvsServiceImpl> _service;
    std::unique_ptr<TDpkvsService::Stub> _stub;
    
    int selected_port_ = 0;
};

TEST_F(ServiceIntegrationTest, PutGetRemoveRoundtrip) {
    // === Put ===
    grpc::ClientContext putContext;
    NKVStore::NService::PutRequest putRequest;
    putRequest.set_key("a");
    putRequest.set_value("1");
    
    NKVStore::NService::PutResponse putResponse;
    auto status1 = _stub->Put(
        &putContext, 
        putRequest, 
        &putResponse);
    
    ASSERT_TRUE(status1.ok());

    // === Get existing ===

    grpc::ClientContext getContext;
    NKVStore::NService::GetRequest getRequest;
    getRequest.set_key("a");
    
    NKVStore::NService::GetResponse getResponse;
    auto status2 = _stub->Get(
        &getContext, 
        getRequest, 
        &getResponse);
    
    ASSERT_TRUE(status2.ok());
    ASSERT_EQ(getResponse.value(), "1");

    // === Remove ===

    grpc::ClientContext removeContext;
    NKVStore::NService::RemoveRequest removeRequest;
    removeRequest.set_key("a");
    NKVStore::NService::RemoveResponse removeResponse;
    
    auto status3 = _stub->Remove(
        &removeContext,
        removeRequest, 
        &removeResponse);
    
    ASSERT_TRUE(status3.ok());
    ASSERT_TRUE(removeResponse.removed());

    // === Get missing ===

    grpc::ClientContext getContext2;
    NKVStore::NService::GetRequest getRequest2;
    getRequest2.set_key("a");
    
    NKVStore::NService::GetResponse getResponse2;
    auto status4 = _stub->Get(
        &getContext2, 
        getRequest2, 
        &getResponse2);
    
    ASSERT_FALSE(status4.ok());
    ASSERT_EQ(status4.error_code(), grpc::StatusCode::NOT_FOUND);
}

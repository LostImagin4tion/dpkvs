#pragma once

#include <dpkvs/network/generated/dpkvs_service.grpc.pb.h>

#include <dpkvs/core/engines/hash_map/engine/store_engine.h>

using grpc::CallbackServerContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerUnaryReactor;
using grpc::Status;
using grpc::StatusCode;

using NKVStore::NCore::NEngine::THashMapStoreEngine;

namespace NKVStore::NService
{

class TDpkvsServiceImpl final
    : public TDpkvsService::CallbackService
{
public:
    TDpkvsServiceImpl();

    explicit TDpkvsServiceImpl(std::unique_ptr<THashMapStoreEngine> storeEngine);

    ServerUnaryReactor* Get(
        CallbackServerContext* context,
        const GetRequest* request,
        GetResponse* response) override final;

    ServerUnaryReactor* Put(
        CallbackServerContext* context,
        const PutRequest* request,
        PutResponse* response) override final;

    ServerUnaryReactor* Remove(
        CallbackServerContext* context,
        const RemoveRequest* request,
        RemoveResponse* response) override final;

private:
    std::unique_ptr<THashMapStoreEngine> _storeEngine;
};

} // namespace NKVStore::NService

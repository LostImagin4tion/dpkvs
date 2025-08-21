#pragma once

#include <dpkvs/network/generated/dpkvs_service.grpc.pb.h>

#include <dpkvs/core/engines/store_engine.h>

using grpc::CallbackServerContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerUnaryReactor;
using grpc::Status;
using grpc::StatusCode;

using NKVStore::NCore::NEngine::IStoreEngine;

namespace NKVStore::NService
{

class TDpkvsServiceImpl final
    : public TDpkvsService::CallbackService
{
public:
    TDpkvsServiceImpl();

    explicit TDpkvsServiceImpl(std::unique_ptr<IStoreEngine> storeEngine);

    ServerUnaryReactor* Get(
        CallbackServerContext* context,
        const TGetRequest* request,
        TGetResponse* response) final;

    ServerUnaryReactor* Put(
        CallbackServerContext* context,
        const TPutRequest* request,
        TPutResponse* response) final;

    ServerUnaryReactor* Remove(
        CallbackServerContext* context,
        const TRemoveRequest* request,
        TRemoveResponse* response) final;

private:
    std::unique_ptr<IStoreEngine> _storeEngine;
};

} // namespace NKVStore::NService

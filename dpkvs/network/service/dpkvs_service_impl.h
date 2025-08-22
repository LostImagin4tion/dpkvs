#pragma once

#include <dpkvs/network/generated/dpkvs_service.grpc.pb.h>

#include <dpkvs/core/engines/store_engine.h>
#include <dpkvs/core/logger/console/console_logger.h>

using grpc::CallbackServerContext;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerUnaryReactor;
using grpc::Status;
using grpc::StatusCode;

using NKVStore::NCore::NEngine::IStoreEngine;
using NKVStore::NCore::NLogger::TConsoleLogger;

namespace NKVStore::NService
{

class TDpkvsServiceImpl final
    : public TDpkvsService::CallbackService
{
public:
    TDpkvsServiceImpl(std::shared_ptr<TConsoleLogger> consoleLogger);

    explicit TDpkvsServiceImpl(
        const std::string& persistenceLogFileName,
        std::shared_ptr<TConsoleLogger> consoleLogger);

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
    const std::string _persistenceLogFileName = "append-only-log.txt";

    std::shared_ptr<TConsoleLogger> _logger;
    std::unique_ptr<IStoreEngine> _storeEngine;
};

} // namespace NKVStore::NService

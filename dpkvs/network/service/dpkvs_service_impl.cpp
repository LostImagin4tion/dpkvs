#include "dpkvs_service_impl.h"

#include <dpkvs/core/engines/hash_map/engine/hash_map_store_engine.h>

#include <utility>

using NKVStore::NCore::NEngine::THashMapStoreEngine;

namespace NKVStore::NService
{

TDpkvsServiceImpl::TDpkvsServiceImpl(std::shared_ptr<TConsoleLogger> consoleLogger)
    : _logger(std::move(consoleLogger))
    , _storeEngine(std::make_unique<THashMapStoreEngine>(_persistenceLogFileName, _logger))
{}

TDpkvsServiceImpl::TDpkvsServiceImpl(
    std::string  persistenceLogFileName,
    std::shared_ptr<TConsoleLogger> consoleLogger)
    : _persistenceLogFileName(std::move(persistenceLogFileName))
    , _logger(std::move(consoleLogger))
    , _storeEngine(std::make_unique<THashMapStoreEngine>(_persistenceLogFileName, _logger))
{}

ServerUnaryReactor* TDpkvsServiceImpl::Get(
    CallbackServerContext* context,
    const TGetRequest* request,
    TGetResponse* response)
{
    _logger->Debug("[TDpkvsServiceImpl] GET - key {}", request->key());

    auto reactor = context->DefaultReactor();

    if (auto value = _storeEngine->Get(request->key()))
    {
        _logger->Debug("[TDpkvsServiceImpl] GET ok - key {}", request->key());

        response->set_value(value->data());
        reactor->Finish(Status::OK);
    } else {
        _logger->Debug("[TDpkvsServiceImpl] GET not_found - key {}", request->key());

        reactor->Finish(Status(StatusCode::NOT_FOUND, "Key not found"));
    }

    return reactor;
}

ServerUnaryReactor* TDpkvsServiceImpl::Put(
    CallbackServerContext* context,
    const TPutRequest* request,
    TPutResponse* response)
{
    _logger->Debug("[TDpkvsServiceImpl] PUT - key {}", request->key());

    auto reactor = context->DefaultReactor();

    _storeEngine->Put(
        request->key(),
        request->value());

    _logger->Debug("[TDpkvsServiceImpl] PUT ok - key {}", request->key());
    reactor->Finish(Status::OK);
    return reactor;
}

ServerUnaryReactor* TDpkvsServiceImpl::Remove(
    CallbackServerContext* context,
    const TRemoveRequest* request,
    TRemoveResponse* response)
{
    _logger->Debug("[TDpkvsServiceImpl] REMOVE - key {}", request->key());

    auto reactor = context->DefaultReactor();

    auto removed = _storeEngine->Remove(request->key());
    response->set_removed(removed);

    if (removed) {
        _logger->Debug("[TDpkvsServiceImpl] REMOVE ok - key {}", request->key());
        reactor->Finish(Status::OK);
    } else {
        _logger->Debug("[TDpkvsServiceImpl] REMOVE not_found - key {}", request->key());
        reactor->Finish(Status(StatusCode::NOT_FOUND, "Key not found"));
    }

    return reactor;
}

} // namespace NKVStore::NService

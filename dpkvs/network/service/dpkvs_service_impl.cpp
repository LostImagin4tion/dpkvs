#include "dpkvs_service_impl.h"

#include <dpkvs/core/engines/hash_map/engine/hash_map_store_engine.h>

using NKVStore::NCore::NEngine::THashMapStoreEngine;

namespace NKVStore::NService
{

TDpkvsServiceImpl::TDpkvsServiceImpl()
    : _storeEngine(std::make_unique<THashMapStoreEngine>())
{}

TDpkvsServiceImpl::TDpkvsServiceImpl(std::unique_ptr<IStoreEngine> storeEngine)
    : _storeEngine(std::move(storeEngine))
{}

ServerUnaryReactor* TDpkvsServiceImpl::Get(
    CallbackServerContext* context,
    const TGetRequest* request,
    TGetResponse* response)
{
    auto reactor = context->DefaultReactor();

    if (auto value = _storeEngine->Get(request->key()))
    {
        response->set_value(value->data());
        reactor->Finish(Status::OK);
    } else {
        reactor->Finish(Status(StatusCode::NOT_FOUND, "Key not found"));
    }

    return reactor;
}

ServerUnaryReactor* TDpkvsServiceImpl::Put(
    CallbackServerContext* context,
    const TPutRequest* request,
    TPutResponse* response)
{
    auto reactor = context->DefaultReactor();

    _storeEngine->Put(
        request->key(),
        request->value());

    reactor->Finish(Status::OK);
    return reactor;
}

ServerUnaryReactor* TDpkvsServiceImpl::Remove(
    CallbackServerContext* context,
    const TRemoveRequest* request,
    TRemoveResponse* response)
{
    auto reactor = context->DefaultReactor();

    auto removed = _storeEngine->Remove(request->key());
    response->set_removed(removed);

    if (removed) {
        reactor->Finish(Status::OK);
    } else {
        reactor->Finish(Status(StatusCode::NOT_FOUND, "Key not found"));
    }

    return reactor;
}

} // namespace NKVStore::NService

#include "dpkvs_service_impl.h"

namespace NKVStore::NService
{

TDpkvsServiceImpl::TDpkvsServiceImpl()
    : _storeEngine(std::make_unique<THashMapStoreEngine>())
{}

TDpkvsServiceImpl::TDpkvsServiceImpl(std::unique_ptr<THashMapStoreEngine> storeEngine)
    : _storeEngine(std::move(storeEngine))
{}

ServerUnaryReactor* TDpkvsServiceImpl::Get(
    CallbackServerContext* context,
    const GetRequest* request,
    GetResponse* response)
{
    auto reactor = context->DefaultReactor();

    if (auto value = _storeEngine->Get(request->key()))
    {
        response->set_value(value->data);
        reactor->Finish(Status::OK);
    } else {
        reactor->Finish(Status(StatusCode::NOT_FOUND, "Key not found"));
    }

    return reactor;
}

ServerUnaryReactor* TDpkvsServiceImpl::Put(
    CallbackServerContext* context,
    const PutRequest* request,
    PutResponse* response)
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
    const RemoveRequest* request,
    RemoveResponse* response)
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

#include "dpkvs_service_impl.h"

namespace NKVStore::NService
{

TDpkvsServiceImpl::TDpkvsServiceImpl()
    : _storeController(std::make_unique<THashMapStoreController>())
{}

TDpkvsServiceImpl::TDpkvsServiceImpl(std::unique_ptr<THashMapStoreController> storeController)
    : _storeController(std::move(storeController))
{}

ServerUnaryReactor* TDpkvsServiceImpl::Get(
    CallbackServerContext* context,
    const GetRequest* request,
    GetResponse* response)
{
    auto reactor = context->DefaultReactor();

    if (auto value = _storeController->Get(request->key()))
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

    _storeController->Put(
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

    auto removed = _storeController->Remove(request->key());
    response->set_removed(removed);

    if (removed) {
        reactor->Finish(Status::OK);
    } else {
        reactor->Finish(Status(StatusCode::NOT_FOUND, "Key not found"));
    }

    return reactor;
}

} // namespace NKVStore::NService

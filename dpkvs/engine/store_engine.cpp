#include "store_engine.h"

#include <mutex>

namespace NKVStore::NEngine {

TStoreEngine::TStoreEngine(TKVStoreMap&& other)
    : _store(std::move(other))
{}

TStoreEngine::TStoreEngine(TStoreEngine&& other) noexcept
{
    std::scoped_lock lock(other._mutex, _mutex);
    _store = std::move(other._store);
}

TStoreEngine& TStoreEngine::operator=(TStoreEngine && other) noexcept
{
    if (this == &other) {
        return *this;
    }

    std::scoped_lock lock(other._mutex, _mutex);
    _store = std::move(other._store);
    return *this;
}

void TStoreEngine::Put(
    std::string key,
    TStorableValue value)
{
    std::unique_lock lock(_mutex);
    _store.insert_or_assign(
        std::move(key),
        std::make_shared<TStorableValue>(std::move(value)));
}

TStorableValuePtr TStoreEngine::Get(const std::string& key) const
{
    std::shared_lock lock(_mutex);

    if (auto iterator = _store.find(key);
        iterator != _store.end())
    {
        return iterator->second;
    }
    return {};
}

bool TStoreEngine::Remove(const std::string &key)
{
    std::unique_lock lock(_mutex);

    return _store.erase(key) > 0;
}

size_t TStoreEngine::Size() const
{
    std::shared_lock lock(_mutex);
    return _store.size();
}

} // namespace NKV::NEngine

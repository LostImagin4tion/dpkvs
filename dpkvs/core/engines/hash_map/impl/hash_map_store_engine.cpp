#include "hash_map_store_engine.h"

#include <mutex>

namespace NKVStore::NCore::NEngine
{

THashMapEngine::THashMapEngine(TKVStoreMap&& other)
    : _store(std::move(other))
{}

THashMapEngine::THashMapEngine(THashMapEngine&& other) noexcept
{
    std::scoped_lock lock(other._mutex, _mutex);
    _store = std::move(other._store);
}

THashMapEngine& THashMapEngine::operator=(THashMapEngine && other) noexcept
{
    if (this == &other) {
        return *this;
    }

    std::scoped_lock lock(other._mutex, _mutex);
    _store = std::move(other._store);
    return *this;
}

void THashMapEngine::Put(
    std::string key,
    TStoreRecord value)
{
    std::unique_lock lock(_mutex);
    _store.insert_or_assign(
        std::move(key),
        std::make_shared<TStoreRecord>(std::move(value)));
}

TStoreRecordPtr THashMapEngine::Get(const std::string& key) const
{
    std::shared_lock lock(_mutex);

    if (auto iterator = _store.find(key);
        iterator != _store.end())
    {
        return iterator->second;
    }
    return {};
}

bool THashMapEngine::Remove(const std::string &key)
{
    std::unique_lock lock(_mutex);

    return _store.erase(key) > 0;
}

size_t THashMapEngine::Size() const
{
    std::shared_lock lock(_mutex);
    return _store.size();
}

} // namespace NKVStore::NCore::NEngine

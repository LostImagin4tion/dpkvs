#include "hash_map_store.h"

#include <mutex>

namespace NKVStore::NCore::NEngine::NRuntime
{

THashMapStore::THashMapStore(TKVStoreMap&& other)
    : _store(std::move(other))
{}

THashMapStore::THashMapStore(THashMapStore&& other) noexcept
{
    std::scoped_lock lock(other._mutex, _mutex);
    _store = std::move(other._store);
}

THashMapStore& THashMapStore::operator=(THashMapStore && other) noexcept
{
    if (this == &other) {
        return *this;
    }

    std::scoped_lock lock(other._mutex, _mutex);
    _store = std::move(other._store);
    return *this;
}

void THashMapStore::Put(
    std::string key,
    TStoreRecord value)
{
    std::unique_lock lock(_mutex);
    _store.insert_or_assign(
        std::move(key),
        std::make_shared<TStoreRecord>(std::move(value)));
}

TStoreRecordPtr THashMapStore::Get(const std::string& key) const
{
    std::shared_lock lock(_mutex);

    if (auto iterator = _store.find(key);
        iterator != _store.end())
    {
        return iterator->second;
    }
    return {};
}

bool THashMapStore::Remove(const std::string &key)
{
    std::unique_lock lock(_mutex);

    return _store.erase(key) > 0;
}

size_t THashMapStore::Size() const
{
    std::shared_lock lock(_mutex);
    return _store.size();
}

} // namespace NKVStore::NCore::NEngine

#include "hash_map_store.h"

#include <mutex>

namespace NKVStore::NCore::NEngine::NRuntime
{

void THashMapStore::Put(
    std::string key,
    TStoreValue value)
{
    auto shardIdx = FindShardFor(key);
    auto storeValue = std::make_shared<TStoreValue>(std::move(value));

    absl::WriterMutexLock lock(&_storeShards[shardIdx].mutex);

    _storeShards[shardIdx].store.insert_or_assign(
        std::move(key),
        std::move(storeValue));
}

TStoreValuePtr THashMapStore::Get(const std::string& key) const
{
    auto shardIdx = FindShardFor(key);
    absl::ReaderMutexLock lock(&_storeShards[shardIdx].mutex);

    if (auto iterator = _storeShards[shardIdx].store.find(key);
        iterator != _storeShards[shardIdx].store.end())
    {
        return iterator->second;
    }
    return {};
}

bool THashMapStore::Remove(const std::string &key)
{
    auto shardIdx = FindShardFor(key);
    absl::WriterMutexLock lock(&_storeShards[shardIdx].mutex);

    return _storeShards[shardIdx].store.erase(key) > 0;
}

size_t THashMapStore::Size() const
{
    size_t total = 0;
    for (const auto& shard : _storeShards) {
        absl::ReaderMutexLock lock(&shard.mutex);
        total += shard.store.size();
    }
    return total;
}

size_t THashMapStore::FindShardFor(absl::string_view key)
{
    return absl::Hash<absl::string_view>{}(key) % _shardsNumber;
}

} // namespace NKVStore::NCore::NEngine

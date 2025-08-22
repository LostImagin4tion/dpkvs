#include "hash_map_store.h"

#include <mutex>

namespace NKVStore::NCore::NEngine::NRuntime
{

THashMapStore::THashMapStore(std::shared_ptr<TConsoleLogger> logger)
    : _logger(std::move(logger))
{}

THashMapStore::THashMapStore(
    TKVStoreMap&& other,
    std::shared_ptr<TConsoleLogger> logger)
    : _logger(std::move(logger))
{
    for (const auto& entry : other) {
        auto shardIdx = FindShardFor(entry.first);
        _storeShards[shardIdx].store.insert_or_assign(entry.first, entry.second);
    }
}

void THashMapStore::Put(
    absl::string_view key,
    TStoreValue value)
{
    auto shardIdx = FindShardFor(key);
    auto storeValue = std::make_shared<TStoreValue>(std::move(value));

    absl::WriterMutexLock lock(&_storeShards[shardIdx].mutex);

    _storeShards[shardIdx].store.insert_or_assign(
        std::string(key),
        std::move(storeValue));

    _logger->Debug("[THashMapStore] PUT ok - key {}, shard idx {}", key, shardIdx);
}

TStoreValuePtr THashMapStore::Get(absl::string_view key) const
{
    auto shardIdx = FindShardFor(key);
    absl::ReaderMutexLock lock(&_storeShards[shardIdx].mutex);

    if (auto iterator = _storeShards[shardIdx].store.find(key);
        iterator != _storeShards[shardIdx].store.end())
    {
        _logger->Debug("[THashMapStore] GET ok - key {}, shard idx {}", key, shardIdx);
        return iterator->second;
    } else {
        _logger->Debug("[THashMapStore] GET not_found - key {}, shard idx {}", key, shardIdx);
    }

    return {};
}

bool THashMapStore::Remove(absl::string_view key)
{
    auto shardIdx = FindShardFor(key);
    absl::WriterMutexLock lock(&_storeShards[shardIdx].mutex);

    auto removed = _storeShards[shardIdx].store.erase(key) > 0;

    if (removed) {
        _logger->Debug("[THashMapStore] REMOVE ok - key {}, shard idx {}", key, shardIdx);
    } else {
        _logger->Debug("[THashMapStore] REMOVE not_found - key {}, shard idx {}", key, shardIdx);
    }
    return removed;
}

size_t THashMapStore::Size() const
{
    size_t total = 0;
    for (const auto& shard : _storeShards) {
        absl::ReaderMutexLock lock(&shard.mutex);
        total += shard.store.size();
    }
    _logger->Debug("[THashMapStore] SIZE {}", total);
    return total;
}

size_t THashMapStore::FindShardFor(absl::string_view key)
{
    return absl::Hash<absl::string_view>{}(key) % _shardsNumber;
}

} // namespace NKVStore::NCore::NEngine::NRuntime

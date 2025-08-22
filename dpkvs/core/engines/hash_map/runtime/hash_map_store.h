#pragma once

#include "dpkvs/core/engines/runtime_store.h"
#include <dpkvs/core/logger/console/console_logger.h>

#include <absl/container/flat_hash_map.h>
#include <absl/synchronization/mutex.h>
#include <array>
#include <memory>

using NKVStore::NCore::NLogger::TConsoleLogger;

using TKVStoreMap = absl::flat_hash_map<std::string, TStoreValuePtr>;

namespace NKVStore::NCore::NEngine::NRuntime
{

class THashMapStore
    : public IRuntimeStore
{
public:
    explicit THashMapStore(std::shared_ptr<TConsoleLogger> logger);

    THashMapStore(
        TKVStoreMap&& other,
        std::shared_ptr<TConsoleLogger> logger);

    THashMapStore(const THashMapStore&) = delete;
    THashMapStore& operator=(const THashMapStore&) = delete;

    THashMapStore(THashMapStore&&) = delete;
    THashMapStore& operator=(THashMapStore&&) = delete;

    ~THashMapStore() override = default;

    void Put(absl::string_view key, TStoreValue value) final;

    TStoreValuePtr Get(absl::string_view key) const final;

    bool Remove(absl::string_view key) final;

    size_t Size() const final;

private:
    struct TStoreShard
    {
        TKVStoreMap store;
        mutable absl::Mutex mutex;
    };

    static size_t FindShardFor(absl::string_view key);

    static constexpr size_t _shardsNumber = 256;
    std::array<TStoreShard, _shardsNumber> _storeShards;

    std::shared_ptr<TConsoleLogger> _logger;
};

} // namespace NKVStore::NCore::NEngine::NRuntime

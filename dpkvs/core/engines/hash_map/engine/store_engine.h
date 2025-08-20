#pragma once

#include <dpkvs/core/engines/hash_map/runtime/hash_map_store.h>
#include <dpkvs/core/engines/hash_map/persistence/append_only_log.h>

using NKVStore::NCore::NRecord::TStoreValue;
using NKVStore::NCore::NEngine::NRuntime::THashMapStore;
using NKVStore::NCore::NEngine::NPersistence::TAppendOnlyLog;

namespace NKVStore::NCore::NEngine
{

class THashMapStoreEngine
{
public:
    THashMapStoreEngine();

    explicit THashMapStoreEngine(const std::string& logFileName);

    THashMapStoreEngine(const THashMapStoreEngine&) = delete;
    THashMapStoreEngine& operator=(const THashMapStoreEngine&) = delete;

    THashMapStoreEngine(THashMapStoreEngine&&) noexcept;
    THashMapStoreEngine& operator=(THashMapStoreEngine&&) noexcept;

    ~THashMapStoreEngine() = default;

    void Put(absl::string_view key, absl::string_view value);

    [[nodiscard]] TStoreValuePtr Get(absl::string_view key) const;

    [[nodiscard]] bool Remove(absl::string_view key);

private:
    std::unique_ptr<THashMapStore> _engine;
    std::unique_ptr<TAppendOnlyLog> _logger;

    absl::Mutex _mutex;
};

} // NKVStore::NCore::NEngine

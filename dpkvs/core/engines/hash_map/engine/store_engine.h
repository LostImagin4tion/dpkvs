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

    explicit THashMapStoreEngine(const std::string& appendOnlyLogFileName);

    THashMapStoreEngine(const THashMapStoreEngine&) = delete;
    THashMapStoreEngine& operator=(const THashMapStoreEngine&) = delete;

    THashMapStoreEngine(THashMapStoreEngine&&) noexcept;
    THashMapStoreEngine& operator=(THashMapStoreEngine&&) noexcept;

    ~THashMapStoreEngine() = default;

    void Put(std::string key, std::string value);

    [[nodiscard]] TStoreValuePtr Get(const std::string& key) const;

    [[nodiscard]] bool Remove(const std::string& key);

private:
    std::unique_ptr<THashMapStore> _engine;
    std::unique_ptr<TAppendOnlyLog> _logger;

    std::mutex _appendOnlyLogMutex;
};

} // NKVStore::NCore::NEngine

#pragma once

#include <dpkvs/core/engines/hash_map/runtime/hash_map_store.h>
#include <dpkvs/core/engines/hash_map/persistence/append_only_log.h>

using NKVStore::NCore::NEngine::THashMapStore;
using NKVStore::NCore::NRecord::TStoreRecord;
using NKVStore::NCore::NEngine::NAppendLog::TAppendOnlyLog;

namespace NKVStore::NCore::NEngine::NController
{

class THashMapStoreController
{
public:
    THashMapStoreController();

    explicit THashMapStoreController(const std::string& appendOnlyLogFileName);

    THashMapStoreController(const THashMapStoreController&) = delete;
    THashMapStoreController& operator=(const THashMapStoreController&) = delete;

    THashMapStoreController(THashMapStoreController&&) noexcept;
    THashMapStoreController& operator=(THashMapStoreController&&) noexcept;

    ~THashMapStoreController() = default;

    void Put(std::string key, std::string value);

    [[nodiscard]] TStoreRecordPtr Get(const std::string& key) const;

    [[nodiscard]] bool Remove(const std::string& key);

private:
    std::unique_ptr<THashMapStore> _engine;
    std::unique_ptr<TAppendOnlyLog> _logger;

    std::mutex _appendOnlyLogMutex;
};

} // NKVStore::NCore::NEngine::NController

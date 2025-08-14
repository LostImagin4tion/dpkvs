#pragma once

#include <dpkvs/core/engines/hash_map/hash_map_store_engine.h>
#include <dpkvs/core/engines/hash_map/append_only_log/append_only_log.h>

using NKVStore::NCore::NEngine::THashMapEngine;
using NKVStore::NCore::NRecord::TStoreRecord;
using NKVStore::NCore::NEngine::NAppendLog::TAppendOnlyLog;

namespace NKVStore::NController
{

class TStoreController
{
public:
    TStoreController();

    explicit TStoreController(const std::string& appendOnlyLogFileName);

    TStoreController(const TStoreController&) = delete;
    TStoreController& operator=(const TStoreController&) = delete;

    TStoreController(TStoreController&&) noexcept;
    TStoreController& operator=(TStoreController&&) noexcept;

    ~TStoreController() = default;

    void Put(std::string key, std::string value);

    [[nodiscard]] TStoreRecordPtr Get(const std::string& key) const;

    [[nodiscard]] bool Remove(const std::string& key);

private:
    std::unique_ptr<THashMapEngine> _engine;
    std::unique_ptr<TAppendOnlyLog> _logger;

    std::mutex _appendOnlyLogMutex;
};

} // NKVStore::NController

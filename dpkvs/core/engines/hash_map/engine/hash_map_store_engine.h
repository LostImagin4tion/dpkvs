#pragma once

#include "dpkvs/core/engines/store_engine.h"

#include <dpkvs/core/engines/hash_map/runtime/hash_map_store.h>
#include <dpkvs/core/engines/hash_map/persistence/append_only_log.h>
#include <dpkvs/core/logger/console/console_logger.h>
 
#include <memory>
#include <string>
#include <mutex>
#include <absl/synchronization/mutex.h>

using NKVStore::NCore::NRecord::TStoreValue;
using NKVStore::NCore::NEngine::NRuntime::THashMapStore;
using NKVStore::NCore::NEngine::NPersistence::TAppendOnlyLog;
using NKVStore::NCore::NLogger::TConsoleLogger;

namespace NKVStore::NCore::NEngine
{

class THashMapStoreEngine
    : public IStoreEngine
{
public:
    explicit THashMapStoreEngine(
        const std::string& logFileName, 
        std::shared_ptr<TConsoleLogger> logger);

    THashMapStoreEngine(const THashMapStoreEngine&) = delete;
    THashMapStoreEngine& operator=(const THashMapStoreEngine&) = delete;

    THashMapStoreEngine(THashMapStoreEngine&&) noexcept;
    THashMapStoreEngine& operator=(THashMapStoreEngine&&) noexcept;

    ~THashMapStoreEngine() override = default;

    void Put(absl::string_view key, absl::string_view value) final;

    [[nodiscard]] TStoreValuePtr Get(absl::string_view key) const final;

    [[nodiscard]] bool Remove(absl::string_view key) final;

private:
    std::shared_ptr<TConsoleLogger> _consoleLogger;

    std::unique_ptr<TAppendOnlyLog> _appendOnlyLog;
    std::unique_ptr<THashMapStore> _engine;

    absl::Mutex _mutex;
};

} // NKVStore::NCore::NEngine

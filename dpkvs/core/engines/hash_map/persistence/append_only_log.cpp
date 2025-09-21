#include "append_only_log.h"

#include <dpkvs/core/store_record/factory/factory.h>

#include <iostream>

using NKVStore::NCore::NRecord::CreatePutRecord;
using NKVStore::NCore::NRecord::CreateRemoveRecord;
using NKVStore::NCore::NRecord::TStoreRecord;

namespace NKVStore::NCore::NEngine::NPersistence
{

TAppendOnlyLog::TAppendOnlyLog(
    const std::string &fileName,
    std::shared_ptr<TConsoleLogger> logger)
    : _consoleLogger(std::move(logger))
    , _logSerializer(std::make_unique<TStoreRecordSerializer>(fileName))
{}

TAppendOnlyLog::TAppendOnlyLog(TAppendOnlyLog&& other) noexcept
    : _consoleLogger(std::move(other._consoleLogger))
    , _logSerializer(std::move(other._logSerializer))
{}

TAppendOnlyLog& TAppendOnlyLog::operator=(TAppendOnlyLog && other) noexcept
{
    _consoleLogger = std::move(other._consoleLogger);
    _logSerializer = std::move(other._logSerializer);
    return *this;
}

void TAppendOnlyLog::AppendPutOperation(
    const std::string& key,
    const TStoreValue& value)
{
    _consoleLogger->Debug("[TAppendOnlyLog] AppendPutOperation - key {}", key);

    auto record = CreatePutRecord(key, value, _logSequenceNumber++);
    _logSerializer->WriteRecord(record);
}

void TAppendOnlyLog::AppendRemoveOperation(const std::string& key)
{
    _consoleLogger->Debug("[TAppendOnlyLog] AppendRemoveOperation - key {}", key);

    auto record = CreateRemoveRecord(key, _logSequenceNumber++);
    _logSerializer->WriteRecord(record);
}

std::unique_ptr<THashMapStore> TAppendOnlyLog::RecoverFromLog()
{
    _consoleLogger->Debug("[TAppendOnlyLog] Started recovering from log");

    auto recoveredStore = absl::flat_hash_map<std::string, TStoreValuePtr>();

    try {
        _logSerializer->EnableReadMode();

        uint64_t currentLsn = 0;
        TStoreRecord record;

        while (_logSerializer->ReadRecord(record)) {
            auto recordLsn = record.log_sequence_number();
            _consoleLogger->Debug("[TAppendOnlyLog] Recovered LSN {}, current {}", recordLsn, currentLsn);

            if (currentLsn > recordLsn) {
                continue;
            }
            currentLsn = recordLsn;

            if (record.has_put_operation()) {
                const auto& put = record.put_operation();
                const auto& key = put.key();
                auto value = put.value();

                recoveredStore[key] = std::make_shared<TStoreValue>(value);

                _consoleLogger->Debug("[TAppendOnlyLog] Recovered PUT with key {}", key);

            } else if (record.has_remove_operation()) {
                const auto& remove = record.remove_operation();
                const auto& key = remove.key();

                recoveredStore.erase(key);

                _consoleLogger->Debug("[TAppendOnlyLog] Recovered REMOVE with key {}", key);
            }
        }
        _logSequenceNumber = currentLsn;
        _consoleLogger->Debug("[TAppendOnlyLog] LSN after recovery {}", _logSequenceNumber);

    } catch (const std::exception& e) {
        _consoleLogger->Error("[TAppendOnlyLog] Error happened while recovering: {}", e.what());
    }

    return std::make_unique<THashMapStore>(std::move(recoveredStore), _consoleLogger);
}

} // namespace NKVStore::NCore::NEngine::NPersistence

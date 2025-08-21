#include "append_only_log.h"

#include <dpkvs/core/store_record/factory/factory.h>

#include <iostream>

using NKVStore::NCore::NRecord::CreatePutRecord;
using NKVStore::NCore::NRecord::CreateRemoveRecord;
using NKVStore::NCore::NRecord::TStoreRecord;

namespace NKVStore::NCore::NEngine::NPersistence
{

TAppendOnlyLog::TAppendOnlyLog()
    : _logSerializer(std::make_unique<TStoreRecordSerializer>())
{}

TAppendOnlyLog::TAppendOnlyLog(const std::string& fileName)
    : _logSerializer(std::make_unique<TStoreRecordSerializer>(fileName))
{}

TAppendOnlyLog::TAppendOnlyLog(TAppendOnlyLog && other) noexcept
    : _logSerializer(std::move(other._logSerializer))
{}

TAppendOnlyLog& TAppendOnlyLog::operator=(TAppendOnlyLog && other) noexcept
{
    _logSerializer = std::move(other._logSerializer);
    return *this;
}

void TAppendOnlyLog::AppendPutOperation(
    const std::string& key,
    const TStoreValue& value)
{
    auto record = CreatePutRecord(key, value, _logSequenceNumber++);
    _logSerializer->WriteRecord(record);
}

void TAppendOnlyLog::AppendRemoveOperation(const std::string& key)
{
    auto record = CreateRemoveRecord(key, _logSequenceNumber++);
    _logSerializer->WriteRecord(record);
}

std::unique_ptr<THashMapStore> TAppendOnlyLog::RecoverFromLog()
{
    auto recoveredStore = absl::flat_hash_map<std::string, TStoreValuePtr>();

    try {
        _logSerializer->EnableReadMode();

        uint64_t currentLsn = 0;
        TStoreRecord record;

        while (_logSerializer->ReadRecord(record)) {
            auto recordLsn = record.log_sequence_number();
            if (currentLsn > recordLsn) {
                continue;
            }
            currentLsn = recordLsn;

            if (record.has_put_operation()) {
                const auto& put = record.put_operation();
                const auto& key = put.key();
                auto value = put.value();

                recoveredStore[key] = std::make_shared<TStoreValue>(value);

            } else if (record.has_remove_operation()) {
                const auto& remove = record.remove_operation();
                const auto& key = remove.key();

                recoveredStore.erase(key);
            }
        }
        _logSequenceNumber = currentLsn;

    } catch (const std::exception& e) {
        std::cerr << "Error reading log: " << e.what() << std::endl;
    }

    return std::make_unique<THashMapStore>(std::move(recoveredStore));
}

} // namespace NKVStore::NCore::NEngine::NPersistence

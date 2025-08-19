#include "append_only_log.h"

#include <dpkvs/core/store_record/factory/factory.h>

#include <iostream>

using NKVStore::NCore::NRecord::CreatePutRecord;
using NKVStore::NCore::NRecord::CreateRemoveRecord;

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
    auto record = CreatePutRecord(key, value);
    _logSerializer->WriteRecord(record);
}

void TAppendOnlyLog::AppendRemoveOperation(const std::string& key)
{
    auto record = CreateRemoveRecord(key);
    _logSerializer->WriteRecord(record);
}

std::unique_ptr<THashMapStore> TAppendOnlyLog::RecoverFromLog()
{
    auto recoveredStore = std::unordered_map<std::string, TStoreValuePtr>();

    try {
        _logSerializer->EnableReadMode();

        while (_logSerializer->ReadyToRead()) {
            auto command = _logSerializer->ReadCommand();

            switch (command) {
                case EStoreEngineOperations::Put: {
                    std::string key = _logSerializer->ReadKey();
                    TStoreValue value = _logSerializer->ReadValue();

                    recoveredStore[std::move(key)] = std::make_shared<TStoreValue>(std::move(value));
                    break;
                }

                case EStoreEngineOperations::Remove: {
                    std::string key = _logSerializer->ReadKey();

                    recoveredStore.erase(key);
                    break;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading log: " << e.what() << std::endl;
    }

    return std::make_unique<THashMapStore>(std::move(recoveredStore));
}

} // namespace NKVStore::NCore::NEngine::NPersistence

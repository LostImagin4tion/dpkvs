#include "append_only_log.h"

#include <iostream>

namespace NKVStore::NCore::NEngine::NAppendLog
{

TAppendOnlyLog::TAppendOnlyLog()
    : _logSerializer(std::make_unique<TAppendLogSerializer>())
{}

TAppendOnlyLog::TAppendOnlyLog(const std::string& fileName)
    : _logSerializer(std::make_unique<TAppendLogSerializer>(fileName))
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
    const TStoreRecord& value)
{
    _logSerializer->WritePutLog(key, value);
}

void TAppendOnlyLog::AppendRemoveOperation(const std::string& key)
{
    _logSerializer->WriteRemoveLog(key);
}

std::unique_ptr<THashMapStore> TAppendOnlyLog::RecoverFromLog()
{
    auto recoveredStore = std::unordered_map<std::string, TStoreRecordPtr>();

    try {
        _logSerializer->EnableReadMode();

        while (_logSerializer->ReadyToRead()) {
            auto command = _logSerializer->ReadCommand();

            switch (command) {
                case EStoreEngineOperations::Put: {
                    std::string key = _logSerializer->ReadKey();
                    TStoreRecord value = _logSerializer->ReadValue();

                    recoveredStore[std::move(key)] = std::make_shared<TStoreRecord>(std::move(value));
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

} // namespace NKVStore::NCore::NEngine::NAppendLog

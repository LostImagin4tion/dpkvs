#include "append_only_log.h"

#include <iostream>

using NKVStore::NEngine::TStoreEngine;
using NKVStore::NEngine::TStorableValue;
using NKVStore::NEngine::TStorableValuePtr;

namespace NKVStore::NAppendLog
{

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
    const TStorableValue& value)
{
    _logSerializer->WritePutLog(key, value);
}

void TAppendOnlyLog::AppendRemoveOperation(const std::string& key)
{
    _logSerializer->WriteRemoveLog(key);
}

std::unique_ptr<TStoreEngine> TAppendOnlyLog::RecoverFromLog()
{
    auto recoveredStore = std::unordered_map<std::string, TStorableValuePtr>();

    try {
        _logSerializer->EnableReadMode();

        while (_logSerializer->ReadyToRead()) {
            EAppendLogOperations command = _logSerializer->ReadCommand();

            switch (command) {
                case EAppendLogOperations::Put: {
                    std::string key = _logSerializer->ReadKey();
                    TStorableValue value = _logSerializer->ReadValue();

                    recoveredStore[std::move(key)] = std::make_shared<TStorableValue>(std::move(value));
                    break;
                }

                case EAppendLogOperations::Remove: {
                    std::string key = _logSerializer->ReadKey();

                    recoveredStore.erase(key);
                    break;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading log: " << e.what() << std::endl;
    }

    return std::make_unique<TStoreEngine>(std::move(recoveredStore));
}

} // namespace NKVStore::NAppendLog

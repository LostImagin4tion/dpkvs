#include "append_only_log.h"

#include <iostream>

namespace NKVStore::NAppendLog
{

TAppendOnlyLog::TAppendOnlyLog(std::string &fileName)
    : _logSerializer(fileName)
{}

TAppendOnlyLog::TAppendOnlyLog(TAppendOnlyLog && other) noexcept
    : _logSerializer(std::move(other._logSerializer))
{}

TAppendOnlyLog& TAppendOnlyLog::operator=(TAppendOnlyLog && other) noexcept
{
    _logSerializer = std::move(other._logSerializer);
    return *this;
}

void TAppendOnlyLog::AppendToLog(
    const EAppendLogOperations& operation,
    const std::string& key,
    std::optional<const NEngine::TStorableValue> value)
{
    switch (operation) {
        case EAppendLogOperations::Put:
            if (!value.has_value()) {
                throw std::invalid_argument("If you use put operation, value must contain not null");
            }
            _logSerializer.WritePutLog(key, value.value());
            break;

        case EAppendLogOperations::Remove:
            _logSerializer.WriteRemoveLog(key);
            break;
    }
}

std::unique_ptr<NEngine::TStoreEngine> TAppendOnlyLog::RecoverFromLog()
{
    auto recoveredStore = std::unordered_map<std::string, NEngine::TStorableValue>();

    try {
        _logSerializer.EnableReadMode();

        while (_logSerializer.ReadyToRead()) {
            EAppendLogOperations command = _logSerializer.ReadCommand();

            switch (command) {
                case EAppendLogOperations::Put: {
                    std::string key = _logSerializer.ReadKey();
                    NEngine::TStorableValue value = _logSerializer.ReadValue();

                    recoveredStore[std::move(key)] = std::move(value);
                    break;
                }

                case EAppendLogOperations::Remove: {
                    std::string key = _logSerializer.ReadKey();

                    recoveredStore.erase(key);
                    break;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error reading log: " << e.what() << std::endl;
    }

    return std::make_unique<NEngine::TStoreEngine>(std::move(recoveredStore));
}

} // namespace NKVStore::NAppendLog

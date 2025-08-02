#pragma once

#include <string>
#include <any>

#include <dpkvs/engine/store_engine.h>
#include <dpkvs/append_only_log/log_serializer.h>
#include <dpkvs/append_only_log/operations.h>

namespace NKVStore::NAppendLog
{

class TAppendOnlyLog
{
public:
    TAppendOnlyLog() = default;

    explicit TAppendOnlyLog(std::string& fileName);

    TAppendOnlyLog(TAppendOnlyLog&) = delete;
    TAppendOnlyLog& operator=(const TAppendOnlyLog&) = delete;

    TAppendOnlyLog(TAppendOnlyLog&&) noexcept;
    TAppendOnlyLog& operator=(TAppendOnlyLog&&) noexcept;

    ~TAppendOnlyLog() = default;

    void AppendToLog(
        const EAppendLogOperations& operation,
        const std::string& key,
        std::optional<const NEngine::TStorableValue> value);

    std::unique_ptr<NEngine::TStoreEngine> RecoverFromLog();

private:
    TAppendLogSerializer _logSerializer;
};

} // namespace NKVStore::NAppendLog

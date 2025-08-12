#pragma once

#include <string>

#include <dpkvs/engine/store_engine.h>
#include <dpkvs/append_only_log/log_serializer.h>
#include <dpkvs/append_only_log/operations.h>

namespace NKVStore::NAppendLog
{

class TAppendOnlyLog
{
public:
    TAppendOnlyLog() = default;

    explicit TAppendOnlyLog(const std::string& fileName);

    TAppendOnlyLog(TAppendOnlyLog&) = delete;
    TAppendOnlyLog& operator=(const TAppendOnlyLog&) = delete;

    TAppendOnlyLog(TAppendOnlyLog&&) noexcept;
    TAppendOnlyLog& operator=(TAppendOnlyLog&&) noexcept;

    ~TAppendOnlyLog() = default;

    void AppendPutOperation(
        const std::string& key,
        const NEngine::TStorableValue value);

    void AppendRemoveOperation(const std::string& key);

    std::unique_ptr<NEngine::TStoreEngine> RecoverFromLog();

private:
    std::unique_ptr<TAppendLogSerializer> _logSerializer;
};

} // namespace NKVStore::NAppendLog

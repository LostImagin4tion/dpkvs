#pragma once

#include <dpkvs/core/store_record/store_record.h>
#include <dpkvs/core/engines/hash_map/append_only_log/log_serializer.h>
#include "dpkvs/core/engines/hash_map/impl/hash_map_store_engine.h"

#include <string>

using NKVStore::NCore::NRecord::TStoreRecord;
using NKVStore::NCore::NEngine::THashMapEngine;

namespace NKVStore::NCore::NEngine::NAppendLog
{

class TAppendOnlyLog
{
public:
    TAppendOnlyLog();

    explicit TAppendOnlyLog(const std::string& fileName);

    TAppendOnlyLog(TAppendOnlyLog&) = delete;
    TAppendOnlyLog& operator=(const TAppendOnlyLog&) = delete;

    TAppendOnlyLog(TAppendOnlyLog&&) noexcept;
    TAppendOnlyLog& operator=(TAppendOnlyLog&&) noexcept;

    ~TAppendOnlyLog() = default;

    void AppendPutOperation(
        const std::string& key,
        const TStoreRecord& value);

    void AppendRemoveOperation(const std::string& key);

    std::unique_ptr<THashMapEngine> RecoverFromLog();

private:
    std::unique_ptr<TAppendLogSerializer> _logSerializer;
};

} // namespace NKVStore::NCore::NEngine::NAppendLog

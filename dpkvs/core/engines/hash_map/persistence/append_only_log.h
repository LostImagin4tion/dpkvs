#pragma once

#include <dpkvs/core/store_value/generated/store_value.pb.h>
#include <dpkvs/core/engines/hash_map/persistence/log_serializer.h>
#include "dpkvs/core/engines/hash_map/runtime/hash_map_store.h"

#include <string>

using NKVStore::NCore::NRecord::TStoreValue;
using NKVStore::NCore::NEngine::NRuntime::THashMapStore;

namespace NKVStore::NCore::NEngine::NPersistence
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
        const TStoreValue& value);

    void AppendRemoveOperation(const std::string& key);

    std::unique_ptr<THashMapStore> RecoverFromLog();

private:
    std::unique_ptr<TAppendLogSerializer> _logSerializer;
};

} // namespace NKVStore::NCore::NEngine::NPersistence

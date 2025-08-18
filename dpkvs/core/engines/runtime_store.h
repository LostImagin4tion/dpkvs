#pragma once

#include <dpkvs/core/store_record/store_record.h>

#include <string>

using NKVStore::NCore::NRecord::TStoreRecord;

using TStoreRecordPtr = std::shared_ptr<const TStoreRecord>;

namespace NKVStore::NCore::NEngine
{

struct IRuntimeStore {
    virtual void Put(std::string key, TStoreRecord value) = 0;

    [[nodiscard]] virtual TStoreRecordPtr Get(const std::string& key) const = 0;

    [[nodiscard]] virtual bool Remove(const std::string& key) = 0;

    [[nodiscard]] virtual size_t Size() const = 0;
};

} // namespace NKVStore::IStoreEngine

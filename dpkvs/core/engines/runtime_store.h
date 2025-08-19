#pragma once

#include <dpkvs/core/store_value/generated/store_value.pb.h>

#include <string>

using NKVStore::NCore::NRecord::TStoreValue;

using TStoreValuePtr = std::shared_ptr<const TStoreValue>;

namespace NKVStore::NCore::NEngine::NRuntime
{

struct IRuntimeStore {
    virtual void Put(std::string key, TStoreValue value) = 0;

    [[nodiscard]] virtual TStoreValuePtr Get(const std::string& key) const = 0;

    [[nodiscard]] virtual bool Remove(const std::string& key) = 0;

    [[nodiscard]] virtual size_t Size() const = 0;
};

} // namespace NKVStore::NCore::NEngine::NRuntime

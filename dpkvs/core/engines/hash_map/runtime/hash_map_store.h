#pragma once

#include "dpkvs/core/engines/runtime_store.h"

#include <memory>
#include <unordered_map>
#include <shared_mutex>

namespace NKVStore::NCore::NEngine
{

using TKVStoreMap = std::unordered_map<std::string, TStoreRecordPtr>;

class THashMapStore
    : public IRuntimeStore
{
public:
    THashMapStore() = default;

    explicit THashMapStore(TKVStoreMap&& other);

    THashMapStore(const THashMapStore&) = delete;
    THashMapStore& operator=(const THashMapStore&) = delete;

    THashMapStore(THashMapStore&&) noexcept;
    THashMapStore& operator=(THashMapStore&&) noexcept;

    ~THashMapStore() = default;

    void Put(std::string key, TStoreRecord value) final;

    TStoreRecordPtr Get(const std::string& key) const final;

    bool Remove(const std::string& key) final;

    size_t Size() const final;

private:
    TKVStoreMap _store;
    mutable std::shared_mutex _mutex;
};

} // namespace NKVStore::NCore::NEngine

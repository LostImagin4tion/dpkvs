#pragma once

#include "dpkvs/core/engines/store_engine.h"

#include <memory>
#include <unordered_map>
#include <shared_mutex>

namespace NKVStore::NCore::NEngine
{

using TKVStoreMap = std::unordered_map<std::string, TStoreRecordPtr>;

class THashMapEngine
    : public IStoreEngine
{
public:
    THashMapEngine() = default;

    explicit THashMapEngine(TKVStoreMap&& other);

    THashMapEngine(const THashMapEngine&) = delete;
    THashMapEngine& operator=(const THashMapEngine&) = delete;

    THashMapEngine(THashMapEngine&&) noexcept;
    THashMapEngine& operator=(THashMapEngine&&) noexcept;

    ~THashMapEngine() = default;

    void Put(std::string key, TStoreRecord value) final;

    TStoreRecordPtr Get(const std::string& key) const final;

    bool Remove(const std::string& key) final;

    size_t Size() const final;

private:
    TKVStoreMap _store;
    mutable std::shared_mutex _mutex;
};

} // namespace NKVStore::NCore::NEngine

#pragma once

#include <dpkvs/engine/storable_value.h>

#include <string>
#include <unordered_map>
#include <shared_mutex>

namespace NKVStore::NEngine
{

using TStorableValuePtr = std::shared_ptr<const TStorableValue>;
using TKVStoreMap = std::unordered_map<std::string, TStorableValuePtr>;

class TStoreEngine
{
public:
    TStoreEngine();

    explicit TStoreEngine(TKVStoreMap&& other);

    TStoreEngine(const TStoreEngine&) = delete;
    TStoreEngine& operator=(const TStoreEngine&) = delete;

    TStoreEngine(TStoreEngine&&) noexcept;
    TStoreEngine& operator=(TStoreEngine&&) noexcept;

    ~TStoreEngine() = default;

    void Put(std::string key, TStorableValue value);

    [[nodiscard]] TStorableValuePtr Get(const std::string& key) const;

    [[nodiscard]] bool Remove(const std::string& key);

    size_t Size() const;

private:
    TKVStoreMap _store;
    mutable std::shared_mutex _mutex;
};

} // namespace NKV::NEngine

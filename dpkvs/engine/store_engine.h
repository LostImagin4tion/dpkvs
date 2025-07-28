//
// Created by Egor Danilov on 28.07.2025.
//

#pragma once

#include <dpkvs/engine/storable_value.h>

#include <string>
#include <unordered_map>
#include <shared_mutex>

namespace NKVStore::Engine
{

    class TStoreEngine
    {
    public:
        TStoreEngine();
        explicit TStoreEngine(std::unordered_map<std::string, TStorableValue>&& other);
        TStoreEngine(const TStoreEngine&) = delete;
        TStoreEngine(TStoreEngine&&) noexcept;
        ~TStoreEngine() = default;

        void operator=(const TStoreEngine&) = delete;
        TStoreEngine& operator=(TStoreEngine&&) noexcept;

        void Put(std::string&& key, TStorableValue&& value);

        [[nodiscard]] std::optional<TStorableValue> Get(const std::string& key) const;

        [[nodiscard]] bool Remove(const std::string& key);

    private:
        std::unordered_map<std::string, TStorableValue> _store;
        mutable std::shared_mutex _mutex;
    };

} // namespace NKV::Engine

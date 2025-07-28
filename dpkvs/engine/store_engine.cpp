//
// Created by Egor Danilov on 28.07.2025.
//

#include "store_engine.h"

namespace NKVStore::Engine {

    TStoreEngine::TStoreEngine()
        : _store(std::unordered_map<std::string, TStorableValue>())
    {}

    TStoreEngine::TStoreEngine(std::unordered_map<std::string, TStorableValue>&& other)
        : _store(std::move(other))
    {}

    TStoreEngine::TStoreEngine(TStoreEngine && other) noexcept
        : _store(std::move(other._store))
    {}

    TStoreEngine& TStoreEngine::operator=(TStoreEngine && other) noexcept
    {
        _store = std::move(other._store);
        return *this;
    }

    void TStoreEngine::Put(
        std::string&& key,
        TStorableValue&& value)
    {
        std::unique_lock lock(_mutex);
        _store.emplace(std::move(key), std::move(value));
    }

    std::optional<TStorableValue> TStoreEngine::Get(const std::string& key) const
    {
        std::shared_lock lock(_mutex);

        if (_store.contains(key)) {
            auto iterator = _store.find(key);
            if (iterator != _store.end()) {
                return iterator->second;
            }
        }
        return std::nullopt;
    }

    bool TStoreEngine::Remove(const std::string &key)
    {
        std::unique_lock lock(_mutex);

        if (_store.contains(key)) {
            _store.erase(key);
            return true;
        }
        return false;
    }

} // namespace NKV::Engine

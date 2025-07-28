//
// Created by Egor Danilov on 28.07.2025.
//

#include "store_engine.h"

namespace NKVStore::Engine {

    TStoreEngine::TStoreEngine()
    {
        _store = std::unordered_map<std::string, std::any>();
    }

    TStoreEngine::TStoreEngine(TStoreEngine && other) noexcept
    {
        _store = std::move(other._store);
    }

    TStoreEngine& TStoreEngine::operator=(TStoreEngine && other) noexcept
    {
        _store = std::move(other._store);
        return *this;
    }

    void TStoreEngine::Put(
        const std::string& key,
        const std::any& value)
    {
        std::unique_lock lock(_mutex);
        _store[key] = value;
    }

    std::optional<const std::any> TStoreEngine::Get(const std::string& key)
    {
        std::shared_lock lock(_mutex);

        if (_store.contains(key)) {
            return _store[key];
        }
        return {};
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

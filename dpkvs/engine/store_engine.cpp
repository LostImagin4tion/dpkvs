//
// Created by Egor Danilov on 28.07.2025.
//

#include "store_engine.h"

namespace NKVStore::Engine {

    TStoreEngine::TStoreEngine() {
        _store = std::unordered_map<std::string, std::any>();
    }

    TStoreEngine::TStoreEngine(TStoreEngine && other) noexcept {
        _store = std::move(other._store);
    }

    TStoreEngine& TStoreEngine::operator=(TStoreEngine && other) noexcept {
        _store = std::move(other._store);
        return *this;
    }

    void TStoreEngine::put(std::string key, std::any value) {
        _store[key] = value;
    }

    const std::any& TStoreEngine::get(std::string key) {
        return _store.at(key);
    }
} // namespace NKV::Engine

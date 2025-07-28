//
// Created by Egor Danilov on 28.07.2025.
//

#pragma once

#include <string>
#include <unordered_map>
#include <any>

namespace NKVStore::Engine {

    class TStoreEngine {
    public:
        TStoreEngine();
        TStoreEngine(const TStoreEngine&) = delete;
        TStoreEngine(TStoreEngine&&) noexcept;
        ~TStoreEngine() = default;

        void operator=(const TStoreEngine&) = delete;
        TStoreEngine& operator=(TStoreEngine&&) noexcept;

        void put(std::string key, std::any value);

        [[nodiscard]] const std::any& get(std::string key);

    private:
        std::unordered_map<std::string, std::any> _store;
    };

} // namespace NKV::Engine

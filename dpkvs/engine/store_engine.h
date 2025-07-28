//
// Created by Egor Danilov on 28.07.2025.
//

#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <shared_mutex>

namespace NKVStore::Engine
{

    class TStoreEngine
    {
    public:
        TStoreEngine();
        TStoreEngine(const TStoreEngine&) = delete;
        TStoreEngine(TStoreEngine&&) noexcept;
        ~TStoreEngine() = default;

        void operator=(const TStoreEngine&) = delete;
        TStoreEngine& operator=(TStoreEngine&&) noexcept;

        void Put(const std::string& key, const std::any& value);

        [[nodiscard]] std::optional<const std::any> Get(const std::string& key);

        bool Remove(const std::string& key);

    private:
        std::unordered_map<std::string, std::any> _store;
        mutable std::shared_mutex _mutex;
    };

} // namespace NKV::Engine

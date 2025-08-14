#pragma once

#include <cstddef>
#include <string>
#include <chrono>
#include <memory>

namespace NKVStore::NCore::NRecord
{

struct TStoreRecord
{
    TStoreRecord() = default;

    explicit TStoreRecord(std::string other);

    TStoreRecord(const TStoreRecord&) = default;
    TStoreRecord(TStoreRecord&&) = default;

    TStoreRecord& operator=(const TStoreRecord&) = default;
    TStoreRecord& operator=(TStoreRecord&&) = default;

    std::string data;
    uint32_t flags = 0;
    std::optional<std::chrono::system_clock::time_point> expiry;
};

} // NKVStore::NCore::NRecord

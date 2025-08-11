#pragma once

#include <cstddef>
#include <optional>
#include <vector>

namespace NKVStore::NEngine
{

struct TStorableValue
{
    TStorableValue() = default;

    explicit TStorableValue(std::vector<uint8_t>&& bytes);

    TStorableValue(const TStorableValue&) = default;
    TStorableValue(TStorableValue&&) = default;

    TStorableValue& operator=(const TStorableValue&) = default;
    TStorableValue& operator=(TStorableValue&&) = default;

    std::vector<uint8_t> binaryData;
    uint32_t flags = 0;
    std::optional<std::chrono::system_clock::time_point> expiry;
};

} // NKVStore::NEngine
